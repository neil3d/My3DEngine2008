
	const char* ndb_query_result::get_string(uint32 col) const
	{
		return m_cur_row[col];
	}

	bool ndb_query_result::get_bool(uint32 col)
	{
		if ( m_cur_row[col] )
			return (1 == atoi(m_cur_row[col]));
		return false;
	}

	uint32 ndb_query_result::get_uint32(uint32 col)
	{
		if ( m_cur_row[col] )
			return static_cast<uint32>(atoi(m_cur_row[col]));
		return 0;
	}

	int32 ndb_query_result::get_int32(uint32 col)
	{
		if( m_cur_row[col] )
			return static_cast<int32>(atoi(m_cur_row[col]));
		return 0;
	}

	uint64 ndb_query_result::get_uint64(uint32 col)
	{
		if( m_cur_row[col] )
			return static_cast<uint64>(_atoi64(m_cur_row[col]));
		return 0;
	}

	int8 ndb_query_result::get_int8(uint32 col)
	{
		if( m_cur_row[col])
			return static_cast<int8>(atoi(m_cur_row[col]));
		return 0;
	}

	uint8 ndb_query_result::get_uint8(uint32 col)
	{
		if( m_cur_row[col] )
			return static_cast<uint8>(atoi(m_cur_row[col]));
		return 0;
	}

	int16 ndb_query_result::get_int16(uint32 col)
	{
		if( m_cur_row[col] )
			return static_cast<int16>(atoi(m_cur_row[col]));
		return 0;
	}

	int16 ndb_query_result::get_uint16(uint32 col)
	{
		if( m_cur_row[col] )
			return static_cast<uint16>(atoi(m_cur_row[col]));
		return 0;
	}

	float ndb_query_result::get_float(uint32 col)
	{
		if( m_cur_row[col] )
			return static_cast<float>(atof(m_cur_row[col]));
		return 0.0f;
	}

	bool ndb_query_result::get_bool(const char* col_name)
	{
		int32 col = (*this).get_col_by_name(col_name);
		if( !(*this).is_valid_col(col) )
			return false;

		if ( m_cur_row[col] )
			return (1 == atoi(m_cur_row[col]));
		return false;
	}

	uint32 ndb_query_result::get_uint32(const char* col_name)
	{
		int32 col = get_col_by_name(col_name);
		if( !is_valid_col(col) )
			return 0;

		if ( m_cur_row[col] )
			return static_cast<uint32>(atoi(m_cur_row[col]));
		return 0;
	}

	int32 ndb_query_result::get_int32(const char* col_name)
	{
		int32 col = get_col_by_name(col_name);
		if( !is_valid_col(col) )
			return 0;

		if( m_cur_row[col] )
			return static_cast<int32>(atoi(m_cur_row[col]));
		return 0;
	}

	uint64 ndb_query_result::get_uint64(const char* col_name)
	{
		int32 col = get_col_by_name(col_name);
		if( !is_valid_col(col) )
			return 0;

		if( m_cur_row[col] )
			return static_cast<uint64>(_atoi64(m_cur_row[col]));
		return 0;
	}

	int8 ndb_query_result::get_int8(const char* col_name)
	{
		int32 col = get_col_by_name(col_name);
		if( !is_valid_col(col) )
			return 0;

		if( m_cur_row[col])
			return static_cast<int8>(atoi(m_cur_row[col]));
		return 0;
	}

	uint8 ndb_query_result::get_uint8(const char* col_name)
	{
		int32 col = get_col_by_name(col_name);
		if( !is_valid_col(col) )
			return 0;

		if( m_cur_row[col] )
			return static_cast<uint8>(atoi(m_cur_row[col]));
		return 0;
	}

	int16 ndb_query_result::get_int16(const char* col_name)
	{
		int32 col = get_col_by_name(col_name);
		if( !is_valid_col(col) )
			return 0;

		if( m_cur_row[col] )
			return static_cast<int16>(atoi(m_cur_row[col]));
		return 0;
	}

	int16 ndb_query_result::get_uint16(const char* col_name)
	{
		int32 col = get_col_by_name(col_name);
		if( !is_valid_col(col) )
			return 0;

		if( m_cur_row[col] )
			return static_cast<uint16>(atoi(m_cur_row[col]));
		return 0;
	}

	float ndb_query_result::get_float(const char* col_name)
	{
		int32 col = get_col_by_name(col_name);
		if( !is_valid_col(col) )
			return 0.f;

		if( m_cur_row[col] )
			return static_cast<float>(atof(m_cur_row[col]));
		return 0.0f;
	}
