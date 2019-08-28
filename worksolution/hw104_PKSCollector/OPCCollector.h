#ifndef OPCCOLLECTOR_H_
#define OPCCOLLECTOR_H_

#include <string>
#include <vector>

#include "hw104_PKSCollector/HW104Collector_ExtInc.h"
#include "HW104CollectorCore_Datatype.h"

struct  COPCCollectorImp;


namespace NKTNCollector{

	class CHW104ConfigAdapter;

	// com operation
	class COPCCollector{
	public:
		COPCCollector(const std::wstring& prog_id,const std::wstring& host,unsigned long update_rate,CHW104ConfigAdapter *);
		~COPCCollector(void);

		bool Connect(void);
		void DisConnect(void);

		bool ReadAll(std::vector<KTNPKSPoint>& points);

		bool WriteItems( KTNPKSPoint& point);

		bool GetStatus(void);
		
	private:
		bool ConnectLocal(void);
		bool ConnectRemote(void);

	public:
		CHW104ConfigAdapter *m_Adpater;
		int m_index;

	private:
		struct COPCCollectorImp *m_Imp;
		unsigned long m_UpdateRate;
		int m_StatusCookie;
	};
}

#endif