#ifndef _NEXUS_SPEED_BASE_TREE_H_
#define _NEXUS_SPEED_BASE_TREE_H_
#include "../resource/nresource.h"
#include "../renderer/nrender_static_mesh_indexed.h"
#include "../renderer/nrender_static_mesh.h"
#include "speed_tree_mesh.h"
#include "speed_tree_material.h"

class CSpeedTreeRT;
namespace nexus
{
	/**
	*	包装一个speed tree base tree对象
	*/
	class nAPI nspeed_base_tree
		: public nresource
	{
	public:
		typedef boost::intrusive_ptr<nspeed_base_tree> ptr;

		explicit nspeed_base_tree(const nstring& name_str);
		virtual ~nspeed_base_tree(void);

		virtual void load_from_file(const resource_location& loc);
		virtual void save_to_file(const resource_location& loc);
		virtual bool ready() const	{	return true;/*todo*/ }

		void advance_speed_wind(float fTime);

		const box_sphere_bounds& get_bounds() const		{	return m_bounding_box; }

		nrender_static_mesh_indexed* get_branches_mesh() const	{	return m_branches_mesh.get();}
		const nmaterial_base* get_branches_material() const	{	return &m_branches_mtl;}

		nrender_static_mesh_indexed* get_frond_mesh() const	{	return m_frond_mesh.get();}
		const nmaterial_base* get_frond_material() const	{	return &m_frond_mtl;}

		const vector2& get_leafcard_angle_scalars() const {	return m_leafcard_angle_scalars;}
		nrender_static_mesh* get_leafcard_lod_mesh(size_t lod) const
		{
			if( lod < m_leafcard_lods.size() )
				return m_leafcard_lods[lod];
			else
				return NULL;
		}
		nrender_static_mesh_indexed* get_leafmesh() const	{	return m_leafmesh.get();}

		const nmaterial_base* get_leaf_material() const	{	return &m_leaf_mtl; }

		const matrix44* get_wind_matrix() const	{	return &m_wnd_mat[0];}
		const float* get_leaf_angle_table() const	{	return &m_vLeafAngleUploadTable[0];}

		bool has_branch() const {	return m_has_branch;}
		bool has_frond() const	{	return m_has_frond;}
		bool has_leafcard() const {	return m_has_leafcard;}
		bool has_leafmesh() const {	return m_has_leafmesh;}

		boost::shared_ptr<CSpeedTreeRT>	get_speed_tree()	{	return m_spt;}
	private:
		void compute_bounds(boost::shared_ptr<CSpeedTreeRT> spt);
		void create_branches(boost::shared_ptr<CSpeedTreeRT> spt);
		void create_frond(boost::shared_ptr<CSpeedTreeRT> spt);
		void create_leafcard(boost::shared_ptr<CSpeedTreeRT> spt);
		void create_leafmesh(boost::shared_ptr<CSpeedTreeRT> spt);
	private:
		boost::shared_ptr<CSpeedTreeRT>	m_spt;

		bool	m_has_branch;
		render_res_ptr<nrender_static_mesh_indexed>	m_branches_mesh;		
		nspt_branch_material	m_branches_mtl;

		bool	m_has_frond;
		render_res_ptr<nrender_static_mesh_indexed>	m_frond_mesh;		
		nspt_frond_material	m_frond_mtl;

		bool	m_has_leafcard, m_has_leafmesh;
		render_res_array<nrender_static_mesh>		m_leafcard_lods;
		render_res_ptr<nrender_static_mesh_indexed>	m_leafmesh;		
		nspt_leaf_material	m_leaf_mtl;

		box_sphere_bounds	m_bounding_box;

		// wind		
		typedef std::vector<const float *> st_vector_cfloat_p;
		typedef std::vector<float> st_vector_float;

		CSpeedWind							m_cGeneralWind;
		st_vector_cfloat_p                  m_vWindMatrices;            // vector of float pointers storing wind matrices for each matrix defined by SpeedWind
		st_vector_float                     m_vWindRustleAngles;        // table of leaf rustle angles shared by all base trees
		st_vector_float                     m_vWindRockAngles;          // table of leaf rock angles shared by all base trees		
		st_vector_float                     m_vLeafAngleUploadTable;    // used as a repository to batch shader constant uploads
		vector2			m_leafcard_angle_scalars;

		matrix44	m_wnd_mat[c_nNumWindMatricesPerSource];
	};
}//namespace nexus
#endif //_NEXUS_SPEED_BASE_TREE_H_