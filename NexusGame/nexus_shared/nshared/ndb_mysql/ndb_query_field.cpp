
#include "nlog_mt.h"
#include "ndb_query_field.h"



namespace nexus{

void ndb_query_field::set_all(char* name, EFieldType field_type, uint32 max_len)
{
	set_name(name);
	set_type(field_type);
	set_max_len(max_len);
}

} //namespace nexus