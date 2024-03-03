#include "pch.h"
#include "MainDoc.h"

void CScan::Processing(int prg)
{
	m_progress = prg;
}

CMainDoc::CMainDoc()
{

}
CMainDoc::~CMainDoc()
{

}
void CMainDoc::SetUp()
{
	
	
}
void CMainDoc::Delete()
{
	for (auto& it : m_scans) {
		delete it;
	}
	m_scans.clear();
}

float CMainDoc::GetSize() const
{
	return m_CanvasSize;
}
std::vector<const CScan*> CMainDoc::GetScans() const
{
	std::vector<const CScan*> rr;
	for (auto& it : m_scans) {
		rr.push_back(it);
	}
	return rr;
}
void CMainDoc::AddScan(PFRECT prect)
{
	CScan* scan = new CScan();
	scan->frc = *prect;
	m_scans.push_back(scan);
}
std::vector<CScan*>& CMainDoc::GetScans()
{
	return m_scans;
}