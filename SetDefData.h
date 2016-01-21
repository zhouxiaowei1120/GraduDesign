#if !defined(AFX_SETDEFDATA_H__0339E9E1_99FE_4F61_912E_EF7BF6B949F4__INCLUDED_)
#define AFX_SETDEFDATA_H__0339E9E1_99FE_4F61_912E_EF7BF6B949F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetDefData.h : header file
//

typedef struct  Point
{
	double lon;			//经度
	double lat;			//纬度
	double dep;			//水深
}Point;


/////////////////////////////////////////////////////////////////////////////
// CSetDefData dialog

class CSetDefData : public CDialog
{
// Construction
public:
	CSetDefData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetDefData)
	enum { IDD = IDD_SETDEFDATA_DIALOG };
	CButton	m_ReadRoadBTN;
	CButton	m_RadioNight;
	CButton	m_RadioDay;
	double	m_factory;
	double	m_hospital;
	double	m_hotel;
	double	m_park;
	double	m_resident;
	double	m_school;
	double	m_shop;
	double	m_work;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetDefData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetDefData)
	afx_msg void OnBTNReadTsuData();
	afx_msg void OnBTNReadRoadData();
	afx_msg void OnBTNReadBuildData();
	afx_msg void OnBTNReadshelterData();
	afx_msg void PopDistribution();
	afx_msg void OnRADIODay();
	afx_msg void OnRADIONight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETDEFDATA_H__0339E9E1_99FE_4F61_912E_EF7BF6B949F4__INCLUDED_)
