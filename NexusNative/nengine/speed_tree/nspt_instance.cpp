#include "StdAfx.h"
#include "nspt_instance.h"

namespace nexus
{
	void nspt_instance::update_lod(float fLod)
	{
		m_fLod = fLod;
		if (fLod <= 0.0f)
		{
			m_fBillboardFadeOut = 1.0f;
			m_bBillboardIsActive = true;
		}
		else
		{
			CSpeedTreeRT::SLodValues sLod;

			base_spt->get_speed_tree()->GetLodValues(sLod, fLod);

			// branches
			m_nBranchLodLevel = char(sLod.m_nBranchActiveLod);
			m_fBranchAlphaValue = sLod.m_fBranchAlphaTestValue;
			nASSERT(m_nBranchLodLevel > -1);

			// fronds
			m_nFrondLodLevel = char(sLod.m_nFrondActiveLod);
			m_fFrondAlphaValue = sLod.m_fFrondAlphaTestValue;
			nASSERT(m_nFrondLodLevel > -1);

			// leaves
			if (sLod.m_anLeafActiveLods[0] > -1)
			{
				m_anLeafLodLevels[0] = char(sLod.m_anLeafActiveLods[0]);
				m_afLeafAlphaValues[0] = sLod.m_afLeafAlphaTestValues[0];
			}
			else
				m_anLeafLodLevels[0] = -1;

			if (sLod.m_anLeafActiveLods[1] > -1)
			{
				m_anLeafLodLevels[1] = char(sLod.m_anLeafActiveLods[1]);
				m_afLeafAlphaValues[1] = sLod.m_afLeafAlphaTestValues[1];
			}
			else
				m_anLeafLodLevels[1] = -1;

			// billboard
			m_fBillboardFadeOut = sLod.m_fBillboardFadeOut;
			m_bBillboardIsActive = m_fBillboardFadeOut > 0.0f;
		}
	}

	nspt_instance::nspt_instance():pos(0,0,0),scale(1)
		, m_fLod(1.0f)
		, m_nBranchLodLevel(0)
		, m_fBranchAlphaValue(84.0f / 255.0f)
		, m_nFrondLodLevel(0)
		, m_fFrondAlphaValue(84.0f / 255.0f)
		, m_bBillboardIsActive(false)
		, m_fBillboardFadeOut(0.0f)
		, owner(NULL)
	{
		mat_set_identity(mat_world);
		set_rotation(0);
		wind_matrix_offset = IdvRandom(0.0f, 1000.0f);

		m_anLeafLodLevels[0] = m_anLeafLodLevels[1] = 0;
		m_afLeafAlphaValues[0] = m_afLeafAlphaValues[1] = 84.0f / 255.0f;
	}

	void nspt_instance::serialize(narchive& ar)
	{
		nSERIALIZE(ar, pos);
		nSERIALIZE(ar, scale);
		nSERIALIZE(ar, rotation);
		nSERIALIZE(ar, wind_matrix_offset);
	}
}//namespace nexus