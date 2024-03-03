#pragma once
#include <vector>

typedef struct {
	float left;
	float top;
	float right;
	float bottom;
}FRECT,*PFRECT;
enum class SCAN_STAT {
	NotCut,
	Cutting,
	Done,
	Bad,
};
class CScan
{
	SCAN_STAT m_status;
public:
	void Processing(int prg);
	void Status(SCAN_STAT stat) {
		m_status = stat; 
	}
	int m_progress;
	
	FRECT frc;
	float Left() const { return frc.left; }
	float Top() const { return frc.top; }
	float Right() const { return frc.right; }
	float Bottom() const { return frc.bottom; }
	float Width() const { return frc.right - frc.left; }
	float Height() const { return frc.bottom - frc.top; }
	float CX() const { return (frc.right + frc.left)/2; }
	float CY() const { return (frc.bottom + frc.top)/2; }
	SCAN_STAT Status() const { return m_status; }
};
class CReadableDoc abstract
{
public:
	virtual std::vector<const CScan*> GetScans() const = 0;
	virtual float GetSize() const = 0;
};
class CMainDoc : public CReadableDoc
{
public:
	CMainDoc();
	~CMainDoc();

	void SetUp();
	void AddScan(PFRECT rect);
	void Delete();

	std::vector<CScan*>& GetScans();
	virtual std::vector<const CScan*> GetScans() const;
	virtual float GetSize() const;
private:
	float m_CanvasSize = 300.0f; //mm
	std::vector<CScan*> m_scans;
};

