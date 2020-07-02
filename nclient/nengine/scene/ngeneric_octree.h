/** 
	ngeneric_octree.h: Generic octree implementation.
*/
#pragma once

#include "../../../ncore/math/vector.h"
#include "../../../ncore/math/matrix.h"
#include "../../../ncore/math/geom_def.h"
#include "../framework/world_define.h"

namespace nexus
{
	enum 
	{
		OCTREE_CHILDXMAX = 0x0004,
		OCTREE_CHILDYMAX = 0x0002,
		OCTREE_CHILDZMAX = 0x0001
	};

	class nfrustum;

	/** An octree node bounding cube.
	*/
	struct octree_node_bounds
	{
		vector3	center;
		float	extent;

		octree_node_bounds(const vector3& inCenter, float inExtent):
			center(inCenter),
			extent(inExtent)
		{}

		// Given a child index and the parent's bounding cube, construct the child's bounding cube.
		octree_node_bounds(const octree_node_bounds& inParentBounds,int inChildIndex)
		{
			extent = inParentBounds.extent * 0.5f;
			center.x = inParentBounds.center.x + (((inChildIndex & OCTREE_CHILDXMAX) >> 1) - 1) * extent;
			center.y = inParentBounds.center.y + (((inChildIndex & OCTREE_CHILDYMAX)     ) - 1) * extent;
			center.z = inParentBounds.center.z + (((inChildIndex & OCTREE_CHILDZMAX) << 1) - 1) * extent;
		}

		// Returns a AABBox representing the cube.
		AABBox get_box() const
		{
			return AABBox(center - vector3(extent,extent,extent),center + vector3(extent,extent,extent));
		}

		// Returns whether this cube is inside the given box.
		bool is_inside_box(const AABBox& inbox) const
		{
			if(center.x - extent < inbox.m_min.x || center.x + extent > inbox.m_max.x)
				return 0;
			else if(center.y - extent < inbox.m_min.y || center.y + extent > inbox.m_max.y)
				return 0;
			else if(center.z - extent < inbox.m_min.z || center.z + extent > inbox.m_max.z )
				return 0;
			else
				return 1;
		}
	};


	/**  generic octree node
	*/
	template<typename _node_type, typename octree_semantics>
	class noctree_node
	{
	public:
		typedef _node_type*				node_type_ptr;
		typedef std::set<node_type_ptr> content_set;
		
		noctree_node()
		{
			for ( int child =0; child < 8; ++child )
			{
				m_children[child] = NULL;
			}
		}

		~noctree_node();

		// operations
		void add_content( node_type_ptr content, octree_node_bounds& bound );
		void remove_content( node_type_ptr content );
		void remove_all();

		content_set& get_curr_contents() { return m_contents; }

		bool is_leaf() 
		{ 
			for( int childindex = 0; childindex < 8; ++childindex )
			{
				if( m_children[childindex] )
					return false;
			}
			return true;
		}

		bool has_child( int childindex )
		{
			return m_children[childindex]!=NULL;
		}

		noctree_node<_node_type,octree_semantics>* get_child( int childindex )
		{
			return m_children[childindex];
		}

	private:
		void store_element(node_type_ptr content, octree_node_bounds& bound, int child );

		/** create array of children node indices that this box overlaps. */
		inline int find_children(const octree_node_bounds& parentbounds, const AABBox& testbox, int* childixs)
		{
			int childcount = 0;

			if(testbox.m_max.x > parentbounds.center.x) // xmax
			{ 
				if(testbox.m_max.y > parentbounds.center.y) // ymax
				{
					if(testbox.m_max.z > parentbounds.center.z) // zmax
						childixs[childcount++] = OCTREE_CHILDXMAX+OCTREE_CHILDYMAX+OCTREE_CHILDZMAX;
					if(testbox.m_min.z <= parentbounds.center.z) // zmin
						childixs[childcount++] = OCTREE_CHILDXMAX+OCTREE_CHILDYMAX          ;
				}

				if(testbox.m_min.y <= parentbounds.center.y) // ymin
				{
					if(testbox.m_max.z > parentbounds.center.z) // zmax
						childixs[childcount++] = OCTREE_CHILDXMAX+          OCTREE_CHILDZMAX;
					if(testbox.m_min.z <= parentbounds.center.z) // zmin
						childixs[childcount++] = OCTREE_CHILDXMAX                    ;
				}
			}

			if(testbox.m_min.x <= parentbounds.center.x) // xmin
			{ 
				if(testbox.m_max.y > parentbounds.center.y) // ymax
				{
					if(testbox.m_max.z > parentbounds.center.z) // zmax
						childixs[childcount++] =           OCTREE_CHILDYMAX+OCTREE_CHILDZMAX;
					if(testbox.m_min.z <= parentbounds.center.z) // zmin
						childixs[childcount++] =           OCTREE_CHILDYMAX          ;	
				}

				if(testbox.m_min.y <= parentbounds.center.y) // ymin
				{
					if(testbox.m_max.z > parentbounds.center.z) // zmax
						childixs[childcount++] =                     OCTREE_CHILDZMAX;
					if(testbox.m_min.z <= parentbounds.center.z) // zmin
						childixs[childcount++] = 0                            ;
				}
			}

			return childcount;
		}

		/** Returns which child node 'testBox' would fit into.
		* Returns -1 if box overlaps any planes, and therefore wont fit into a child.
		* Assumes testBox would fit into this (parent) node.
		*/
		inline int find_child(const octree_node_bounds& parentbounds, const AABBox& testbox)
		{
			int result = 0;

			if(testbox.m_min.x > parentbounds.center.x)
				result |= OCTREE_CHILDXMAX;
			else if(testbox.m_max.x > parentbounds.center.x)
				return -1;

			if(testbox.m_min.y > parentbounds.center.y)
				result |= OCTREE_CHILDYMAX;
			else if(testbox.m_max.y > parentbounds.center.y)
				return -1;

			if(testbox.m_min.z > parentbounds.center.z)
				result |= OCTREE_CHILDZMAX;
			else if(testbox.m_max.z > parentbounds.center.z)
				return -1;

			return result;
		}

	private:
		// 节点内容
		content_set					m_contents;
		// Child nodes. If all NULL, this is a leaf.
		noctree_node<_node_type,octree_semantics>*	m_children[8];
	};


	/** generic octree
	*/
	template<typename _node_type, typename octree_semantics>
	class noctree
	{
	public:
		// type define
		typedef _node_type*									node_type_ptr;
		typedef noctree_node<_node_type, octree_semantics>*	octreenode_ptr;
	
		// constructions 
		noctree() :
			m_root_bound( vector3(0,0,0), HALF_WORLD_MAX ),
			m_root( new noctree_node<_node_type,octree_semantics>() )
		{
		}
		noctree(vector3 orgin, float extent ):
			m_root_bound( orgin, extent ),
			m_root( new noctree_node<_node_type,octree_semantics>() )
		{}

		~noctree();
		
		// operations
		void add_content( node_type_ptr content );
		void remove_content( node_type_ptr content );
		void move_content( node_type_ptr content );
		void remove_all();

		/** An octree node const iterator. 
		*/
		class const_iterator
		{
		public:
			/** A reference to an octree node
			*/
			struct octreenode_ref
			{
				octreenode_ptr				_node;
				octree_node_bounds			_bound;

				octreenode_ref() : _node( NULL )
				{
				}

				octreenode_ref( octreenode_ptr node, const octree_node_bounds& bound ) :
					_node(node), _bound(bound)
				{
				}
			};

			const_iterator( const noctree& octrant ) :
					m_current_node( octreenode_ref(octrant.m_root,octrant.m_root_bound) )
			{
			} 

			bool has_pending_node() const
			{
				return (m_current_node._node != NULL);
			}

			void advance()
			{
				if( m_node_stack.size() == 0 )
				{
					m_current_node._node = NULL;
				}
				else
				{
					m_current_node = m_node_stack.back();
					m_node_stack.pop_back();
				}
			}

			// 优化 
			void push_children( int child )
			{
				if( m_current_node._node != NULL 
					&& (m_current_node._node)->has_child(child) )
				{
					m_node_stack.push_back( octreenode_ref( 
						(m_current_node._node)->get_child(child), 
						octree_node_bounds(m_current_node._bound,child) )
						);
				}
			}

			const octreenode_ptr get_curr_node()const { return m_current_node._node; }
			const octree_node_bounds& get_curr_boud() const { return m_current_node._bound; }
		private:
			octreenode_ref				m_current_node;
			std::list<octreenode_ref>	m_node_stack;
		};

	public:
		octree_node_bounds				m_root_bound;
		octreenode_ptr					m_root;
	};

	//////////////////////////////////////////////////////////////////////////
	// class noctree
	template<typename _node_type, typename octree_semantics>
	noctree<_node_type,octree_semantics>::~noctree()
	{
		if( m_root ) delete m_root;
	}

	template<typename _node_type, typename octree_semantics>
	void noctree<_node_type,octree_semantics>::add_content( node_type_ptr content )
	{
		nASSERT( m_root != NULL );
		nASSERT( content != NULL );
		m_root->add_content( content, m_root_bound );
	}

	template<typename _node_type, typename octree_semantics>
	void noctree<_node_type,octree_semantics>::remove_content( node_type_ptr content )
	{
		nASSERT( m_root != NULL );
		nASSERT( content != NULL );
		if( content->get_octreenode() )
		{
			content->get_octreenode()->remove_content( content );
		}
		else
		{
			m_root->remove_content( content );
		}
	}

	template<typename _node_type, typename octree_semantics>
	void noctree<_node_type,octree_semantics>::move_content( node_type_ptr content )
	{
		nASSERT( m_root != NULL );
		nASSERT( content != NULL );
		if( content->get_octreenode() )
		{
			// remove this content
			content->get_octreenode()->remove_content( content );
		}
		else
		{
			m_root->remove_content( content );
		}

		// re-add to octree
		add_content( content );
	}

	template<typename _node_type, typename octree_semantics>
	void noctree<_node_type,octree_semantics>::remove_all()
	{
		nASSERT( m_root != NULL );
		m_root->remove_all( );
	}

	// class noctree_node
	template<typename _node_type, typename octree_semantics>
	noctree_node<_node_type,octree_semantics>::~noctree_node()
	{
		for ( int child =0; child < 8; ++child )
		{
			if( m_children[child] ) 
			{ 
				delete m_children[child];
				m_children[child] = NULL;
			}
		}
	}

	template<typename _node_type, typename octree_semantics>
	void noctree_node<_node_type,octree_semantics>::add_content(  node_type_ptr content, octree_node_bounds& bound  )
	{
		// If there are no children, or this primitives bounding box completely contains this nodes
		// bounding box, store the actor at this node.
		int childIx = find_child( bound, octree_semantics::get_bounding_box( content ));
		if( childIx == -1 || !m_children[ childIx ] )
		{
			// may be split this node
			store_element( content, bound, childIx );
		}
		else
		{
			// give it
			octree_node_bounds child_bound = octree_node_bounds(bound,childIx);
			m_children[childIx]->add_content(content, child_bound );
		}
	}

	template<typename _node_type, typename octree_semantics>
	void noctree_node<_node_type,octree_semantics>::store_element(node_type_ptr content, octree_node_bounds& bound, int child )
	{
		if( child != -1
			&& m_children[child] == NULL
			&& 0.5f * bound.extent > octree_semantics::MIN_NODE_SIZE 
			)
		{
			// Allocate memory for children nodes.
			m_children[child] = new noctree_node;

			// Now we need to remove each actor from this node and re-check it,
			// in case it needs to move down the Octree.
			content_set pend_contents = m_contents;
			m_contents.clear();

			// add content to child node
			octree_node_bounds child_bound = octree_node_bounds(bound,child);
			m_children[child]->add_content( content, child_bound );

			// Re-add all of the node's elements, potentially creating children of this node for them.
			content_set::const_iterator  it = pend_contents.begin();
			while ( it != pend_contents.end() )
			{
				add_content( content, bound );
			}
		}
		else
		{
			// save this element to this node
			m_contents.insert( content );

			content->set_octreenode( this );
		}
	}

	template<typename _node_type, typename octree_semantics>
	void noctree_node<_node_type,octree_semantics>::remove_content( node_type_ptr content )
	{
		content_set::iterator it = m_contents.find( content );
		if ( it != m_contents.end() )
		{
			m_contents.erase( content );
			// set content octree node ref to NULL
			content->set_octreenode( NULL );
		}
		else
		{
			for ( int i=0; i<8; ++i)
			{
				if( m_children[i] )
				{
					m_children[i]->remove_content( content );
				}
			}
		}
	}

	template<typename _node_type, typename octree_semantics>
	void noctree_node<_node_type,octree_semantics>::remove_all()
	{
		m_contents.clear();

		for ( int i=0; i<8; ++i )
		{
			if( m_children[i] )
			{
				m_children[i]->remove_all();
			}
		}
	}

}	// namespace nexus

