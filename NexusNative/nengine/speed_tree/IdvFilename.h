///////////////////////////////////////////////////////////////////////  
//
//  *** INTERACTIVE DATA VISUALIZATION (IDV) CONFIDENTIAL AND PROPRIETARY INFORMATION ***
//
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Interactive Data Visualization, Inc. and
//  may not be copied, disclosed, or exploited except in accordance with 
//  the terms of that agreement.
//
//      Copyright (c) 2003-2008 IDV, Inc.
//      All rights reserved in all media.
//
//      IDV, Inc.
//      Web: http://www.idvinc.com


///////////////////////////////////////////////////////////////////////  
//  Preprocessor

#pragma once


///////////////////////////////////////////////////////////////////////  
//  IdvExtension 

inline std::string IdvExtension(const std::string& strFilename, char chExtensionChar = '.')
{
	std::string::size_type uiPos = strFilename.find_last_of(chExtensionChar);
	if (uiPos != std::string::npos)
		return strFilename.substr(uiPos + 1);
	else
		return std::string("");
}


///////////////////////////////////////////////////////////////////////  
//  IdvNoExtension 

inline std::string IdvNoExtension(const std::string& strFilename, char chExtensionChar = '.')
{
	std::string::size_type uiPos = strFilename.find_last_of(chExtensionChar);

	if (uiPos != std::string::npos)
		return strFilename.substr(0, uiPos);
	else
		return strFilename;
}


///////////////////////////////////////////////////////////////////////  
//  IdvPath 

inline std::string IdvPath(const std::string& strFilename, std::string strDelimiters = "/\\")
{
	std::string::size_type uiPos = strFilename.find_last_of(strDelimiters);

	if (uiPos != std::string::npos)
		return strFilename.substr(0, uiPos + 1);
	else
		return std::string("");
}


///////////////////////////////////////////////////////////////////////  
//  IdvNoPath 

inline std::string IdvNoPath(const std::string& strFilename, std::string strDelimiters = "/\\")
{
	std::string::size_type uiPos = strFilename.find_last_of(strDelimiters);
	if (uiPos != std::string::npos)
		return strFilename.substr(uiPos + 1);
	else
		return strFilename;
}

