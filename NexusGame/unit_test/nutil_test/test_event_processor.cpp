#include "ncommon.h"
#include "nevent_processor.h"
#include "nevent_args.h"

#include <boost/test/unit_test.hpp>

using namespace nexus;

class test_event
{
public:
	void on_pre_item_use1(const nevent_base_args* args)
	{
		const nevent_pre_item_use_args* data_ptr = static_cast<const nevent_pre_item_use_args*>(args);
		assert(data_ptr->event_id == EEI_PRE_ITEM_USE);
		assert(data_ptr->item_id == 10);
		assert(data_ptr->item_guid == 1000);
	}

	void on_pre_item_use2(const nevent_base_args* args)
	{
		const nevent_pre_item_use_args* data_ptr = static_cast<const nevent_pre_item_use_args*>(args);
		assert(data_ptr->event_id == EEI_PRE_ITEM_USE);
		assert(data_ptr->item_id == 10);
		assert(data_ptr->item_guid == 1000);
	}

	void on_pre_item_use3(const nevent_base_args* args)
	{
		const nevent_pre_item_use_args* data_ptr = static_cast<const nevent_pre_item_use_args*>(args);
		assert(data_ptr->event_id == EEI_PRE_ITEM_USE);
		assert(data_ptr->item_id == 10);
		assert(data_ptr->item_guid == 1000);
	}

	void on_item_used1(const nevent_base_args* args)
	{
		const nevent_item_used_args* data_ptr = static_cast<const nevent_item_used_args*>(args);
		assert(data_ptr->event_id == EEI_ITEM_USED);
		assert(data_ptr->item_id == 10);
		assert(data_ptr->item_guid == 1000);
	}

	void on_item_used2(const nevent_base_args* args)
	{
		const nevent_item_used_args* data_ptr = static_cast<const nevent_item_used_args*>(args);
		assert(data_ptr->event_id == EEI_ITEM_USED);
		assert(data_ptr->item_id == 10);
		assert(data_ptr->item_guid == 1000);
	}

	void on_npc_killed1(const nevent_base_args* args)
	{
		const nevent_npc_killed_args* data_ptr = static_cast<const nevent_npc_killed_args*>(args);
		assert(data_ptr->event_id == EEI_NPC_KILLED);
		assert(data_ptr->npc_info_id == 20);
	}

	void on_npc_killed2(const nevent_base_args* args)
	{
		const nevent_npc_killed_args* data_ptr = static_cast<const nevent_npc_killed_args*>(args);
		assert(data_ptr->event_id == EEI_NPC_KILLED);
		assert(data_ptr->npc_info_id == 20);
	}
};

BOOST_AUTO_TEST_CASE(test_event_processor)
{
	nevent_processor event_proprocessor;
	test_event test;

	event_proprocessor.register_event_handler(EEI_PRE_ITEM_USE,		boost::bind(&test_event::on_pre_item_use1, &test, _1));
	event_proprocessor.register_event_handler(EEI_ITEM_USED,		boost::bind(&test_event::on_item_used1, &test, _1));
	event_proprocessor.register_event_handler(EEI_PRE_ITEM_USE,		boost::bind(&test_event::on_pre_item_use2, &test, _1));
	event_proprocessor.register_event_handler(EEI_ITEM_USED,		boost::bind(&test_event::on_item_used2, &test, _1));
	event_proprocessor.register_event_handler(EEI_PRE_ITEM_USE,		boost::bind(&test_event::on_pre_item_use3, &test, _1));
	event_proprocessor.register_event_handler(EEI_NPC_KILLED,		boost::bind(&test_event::on_npc_killed1, &test, _1));
	event_proprocessor.register_event_handler(EEI_NPC_KILLED,		boost::bind(&test_event::on_npc_killed2, &test, _1));

	nevent_pre_item_use_args pre_item_use;
	pre_item_use.item_id = 10;
	pre_item_use.item_guid = 1000;
	event_proprocessor.send_event(&pre_item_use, sizeof(pre_item_use));

	nevent_npc_killed_args npc_killed;
	npc_killed.npc_info_id = 20;
	event_proprocessor.send_event(&npc_killed, sizeof(npc_killed));

	nevent_item_used_args item_uesd;
	item_uesd.item_id = 10;
	item_uesd.item_guid = 1000;
	event_proprocessor.send_event(&item_uesd, sizeof(item_uesd));

	// post
	event_proprocessor.post_event(1000, &pre_item_use, sizeof(pre_item_use));
	event_proprocessor.post_event(1000, &npc_killed, sizeof(npc_killed));
	event_proprocessor.post_event(1000, &item_uesd, sizeof(item_uesd));

	event_proprocessor.update(200);
	event_proprocessor.update(200);
	event_proprocessor.update(200);
	event_proprocessor.update(200);
	event_proprocessor.update(200);
	event_proprocessor.update(200);

	event_proprocessor.post_event(1000, &pre_item_use, sizeof(pre_item_use));
	event_proprocessor.post_event(1000, &npc_killed, sizeof(npc_killed));
	event_proprocessor.post_event(1000, &item_uesd, sizeof(item_uesd));


	typedef std::multimap<uint32, uint32>	test_insert_map;
	typedef std::list<uint32>				test_insert_list;

	test_insert_map test_map;
	test_map.insert(std::make_pair(1,1));
	test_map.insert(std::make_pair(2,2));
	test_map.insert(std::make_pair(3,3));
	test_map.insert(std::make_pair(4,4));
	test_map.insert(std::make_pair(5,5));
	test_map.insert(std::make_pair(6,6));
	test_map.insert(std::make_pair(7,7));
	test_map.insert(std::make_pair(8,8));

	int32 count = 0;

	for (test_insert_map::const_iterator iter = test_map.begin(); iter != test_map.end(); ++iter)
	{
		if (iter->second == 5)
		{
			test_map.insert(std::make_pair(3,4));
}

		++count;
	}


	test_insert_list test_list;

	test_list.push_back(1);
	test_list.push_back(2);
	test_list.push_back(3);
	test_list.push_back(4);
	test_list.push_back(5);
	test_list.push_back(6);

	count = 0;

	for (test_insert_list::const_iterator iter = test_list.begin(); iter != test_list.end(); ++iter)
	{
		if (*iter == 6)
		{
			test_list.push_back(8);
		}

		++count;
	}

	return;
}
