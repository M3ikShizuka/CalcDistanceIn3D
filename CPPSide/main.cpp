#include "main.hpp"
#include "CEntity.hpp"
#include <math.h>
#include <Windows.h>
#include <iomanip>
#include <conio.h>

// Global time vars
double g_dTimeProgramInitData = 0.0,
g_dTimeProgramProcessingData = 0.0,
g_dTimeProgramFreeData = 0.0,
g_dTimeProgramWorkFull = 0.0;

void InitEntities(CEntity** ppEntityList, CEntity** ppMyEntity);
void ProcessData(const CEntity* pEntityList, const CEntity* pMyEntity);
const float CalcDist(const CEntity* pEntity1, const CEntity* pEntity2);
void FreeEntities(CEntity** ppEntityList, CEntity** ppMyEntity);
const double CalcTime(const unsigned long long ullStartTick, const unsigned long long ullEndTick);
void PrintWorkInfo();

int main()
{
	CEntity* pEntityList = nullptr,
		* pMyEntity = nullptr;
	unsigned long long ullStartTick = 0,
		ullEndTick = 0;

	ullStartTick = GetTickCount64();

	InitEntities(&pEntityList, &pMyEntity);
	ProcessData(pEntityList, pMyEntity);
	FreeEntities(&pEntityList, &pMyEntity);

	ullEndTick = GetTickCount64();
	g_dTimeProgramWorkFull = CalcTime(ullStartTick, ullEndTick);

	PrintWorkInfo();

	_getch();
	return 0;
}

void InitEntities(CEntity** ppEntityList, CEntity** ppMyEntity)
{
	unsigned long long ullStartTick = 0,
		ullEndTick = 0;
	ullStartTick = GetTickCount64();

	// Init my entity.
	*ppMyEntity = new CEntity(1.0f, 2.0f, 3.0f);
	
#ifdef DATA_LIST
	// Init list of entities.
	* ppEntityList = new CEntity[ELEMENT_SIZE];

	for (int iIndex = 0; iIndex < ELEMENT_SIZE; iIndex++)
	{
		CEntity* pEntity = &(*ppEntityList)[iIndex];
		pEntity->SetPos(31.0f, 13.0f, 33.0f);
	}
#else
	// Init list of pointers on entities.
	* ppEntityList = new CEntity[ELEMENT_SIZE];

	for (int iIndex = 0; iIndex < ELEMENT_SIZE; iIndex++)
	{
		CEntity* pEntity = new CEntity(31.0f, 13.0f, 33.0f);
		reinterpret_cast<CEntity**>(*ppEntityList)[iIndex] = pEntity;
	}
#endif

	ullEndTick = GetTickCount64();
	g_dTimeProgramInitData = CalcTime(ullStartTick, ullEndTick);
}

void ProcessData(const CEntity* pEntityList, const CEntity* pMyEntity)
{
	unsigned long long ullStartTick = 0,
		ullEndTick = 0;

	ullStartTick = GetTickCount64();

	// Alloc storage.
	float* pfDist = new float[ELEMENT_SIZE];

	for (int iIndex = 0; iIndex < ELEMENT_SIZE; iIndex++)
	{
#ifdef DATA_LIST
		// Process list of entities.
		const CEntity* pEntity = &pEntityList[iIndex];
		pfDist[iIndex] = CalcDist(pMyEntity, pEntity);
#else
		// Process list of pointers on entities.
		const CEntity* pEntity = reinterpret_cast<CEntity**>(const_cast<CEntity*>(pEntityList))[iIndex];
		pfDist[iIndex] = CalcDist(pMyEntity, pEntity);
#endif
	}

	ullEndTick = GetTickCount64();
	g_dTimeProgramProcessingData = CalcTime(ullStartTick, ullEndTick);

	// Free storage.
	if (pfDist)
	{
		delete[] pfDist;
		pfDist = nullptr;
	}
}

const float CalcDist(const CEntity* pEntity1, const CEntity* pEntity2)
{
	// Сalculate the distance between points in a three-dimensional coordinate system.
	// dist = ((x2 - x1)^2 + (y2 - y1)^2 + (z2 - z1)^2)^1/2
	const float fExponent = 2.0f;
	const SPos pos1 = pEntity1->GetPos(),
		pos2 = pEntity2->GetPos();

	return sqrtf(
		powf((pos2.x - pos1.x), fExponent) +
		powf((pos2.y - pos1.y), fExponent) +
		powf((pos2.z - pos1.z), fExponent)
	);
}

void FreeEntities(CEntity** ppEntityList, CEntity** ppMyEntity)
{
	unsigned long long ullStartTick = 0,
		ullEndTick = 0;

	ullStartTick = GetTickCount64();

	// Free allocated memory for entities.
	if (*ppEntityList)
	{
#ifndef DATA_LIST
		// Free pointers of list.
		for (int iIndex = 0; iIndex < ELEMENT_SIZE; iIndex++)
		{
			CEntity* pEntity = ((*(CEntity***)ppEntityList)[iIndex]);
			delete pEntity;
			pEntity = nullptr;
		}
#endif
		// Free list.
		delete[] *ppEntityList;
		*ppEntityList = nullptr;
	}

	if (*ppMyEntity)
	{
		delete *ppMyEntity;
		*ppMyEntity = nullptr;
	}

	ullEndTick = GetTickCount64();
	g_dTimeProgramFreeData = CalcTime(ullStartTick, ullEndTick);
}

const double CalcTime(const unsigned long long ullStartTick, const unsigned long long ullEndTick)
{
	return (double)(ullEndTick - ullStartTick) / 1000.0;
}

void PrintWorkInfo()
{
	int precision = std::numeric_limits<double>::max_digits10;

	std::cout <<
#ifdef DATA_LIST
		"List of entities\n"
#else
		"List of pointers on entities\n"
#endif
		<< "Data amount: " << ELEMENT_SIZE << "\n"
		<< "Data initiation: " << std::setprecision(precision) << g_dTimeProgramInitData << " sec\n"
		<< "Data processing: " << g_dTimeProgramProcessingData << " sec\n"
		<< "Data release: " << g_dTimeProgramFreeData << " sec\n"
		<< "Total run time: " << g_dTimeProgramWorkFull << " sec\n";
}