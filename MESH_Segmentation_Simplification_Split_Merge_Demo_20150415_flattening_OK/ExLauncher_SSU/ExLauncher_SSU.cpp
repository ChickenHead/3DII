#pragma once

#include "ExLauncher_SSU.h"
#include <math.h>
#include <assert.h>

#include "SSU/PolyhedronUtils.h"
#include "SSU/SVXPrimitveBuilder.h"

#include <Windows.h>

static int demo_step = 0;
static int iNumRequiredMeshes = 518; 
static double sensitivity = cos(30 * 3.14/180.0);
LARGE_INTEGER liCounter1, liCounter2, liFrequency;

static vxfloat4 f4ColorObjects[24] = {
	
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 0
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 1
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 2
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 3

	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 4
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 5
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 6
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 7

	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 8
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 9
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 10
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 11

	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 12
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 13
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 14
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 15

	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 16
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 17
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 18
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 19

	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 20
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 21
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1), // 22
	vxfloat4(rand()%101/100.0, rand()%101/100.0, rand()%101/100.0, 1)  // 23

};

bool compare(Polyhedron p1, Polyhedron p2)
{
	return p1.size_of_facets() > p2.size_of_facets();
}

void segmentationFromPolyhedron(Polyhedron & poly, double sensitivity, std::vector<Polyhedron> & polyvector);
int flattenPolyhedron(Polyhedron& poly)
{
	vxfloat3 average_normal = vxfloat3(0, 0, 0);

	std::for_each(poly.facets_begin(), poly.facets_end(), Facet_normal());

	for (Facet_iterator fit = poly.facets_begin(); fit != poly.facets_end(); ++fit) {
        //Polyhedron::Halfedge_around_facet_circulator facet_circulator = fit->facet_begin();
    	average_normal.x += fit->normal().x();        
		average_normal.y += fit->normal().y();        
		average_normal.z += fit->normal().z();        
	}
	//average_normal/= poly.size_of_facets();
	average_normal /= sqrt(average_normal.x*average_normal.x + average_normal.y*average_normal.y + average_normal.z*average_normal.z);

	float average_dot = 0;
	vxfloat3 pos;
	for (Polyhedron::Point_iterator pit = poly.points_begin(); pit != poly.points_end(); ++pit) {
		pos = vxfloat3(pit->x(), pit->y(), pit->z());
		float dot = pos.x*average_normal.x + pos.y*average_normal.y+ pos.z*average_normal.z;
		average_dot += dot;
	}
	average_dot /= poly.size_of_vertices();

	for (Polyhedron::Point_iterator pit = poly.points_begin(); pit != poly.points_end(); ++pit) {
		pos = vxfloat3(pit->x(), pit->y(), pit->z());
		float dot = pos.x*average_normal.x + pos.y*average_normal.y+ pos.z*average_normal.z;
		float diff = average_dot-dot;
		vxfloat3 new_pos = pos + average_normal*diff;
		vxfloat3 translation_amount = new_pos-pos;
		//CGAL::Aff_transformation_3< Kernel > translation_matrix(CGAL::TRANSLATION, Vector_3(translation_amount.x, translation_amount.y, translation_amount.z));
		//pit->transform(translation_matrix);
		*pit = Point_3(new_pos.x, new_pos.y, new_pos.z);
		//*pit = Point_3(pos.x+2, pos.y, pos.z);

	}

	return 0;
}

//std::vector<Polyhedron> polyhedron_vector;
bool demo_step_0_segmentation(SVXModuleParameters* psvxModuleParameter)
{
	CVXVObjectPrimitive* pCVxMeshIn = (CVXVObjectPrimitive*)psvxModuleParameter->GetVXObject(vxrObjectTypePRIMITIVE, true, 0);		
	vector<CVXObject*> vtrObjList;
	int iNumMeshes = psvxModuleParameter->GetVXObjectList(&vtrObjList, vxrObjectTypePRIMITIVE, false);	

	// 1) input mesh -> polyhedron
	Polyhedron polyhedron;
	const SVXPrimitiveDataArchive* psvxMeshArchiveInput = pCVxMeshIn->GetPrimitiveArchive();
	QueryPerformanceFrequency(&liFrequency);
	QueryPerformanceCounter(&liCounter1);
	PolyhedronUtils::generatePolyhedronFromSVXPrimitive(polyhedron, psvxMeshArchiveInput);
	QueryPerformanceCounter(&liCounter2);
	//cout<<"1) Time : "<<(double)(liCounter2.QuadPart - liCounter1.QuadPart) / (double)liFrequency.QuadPart<<endl;

	//////////
	pCVxMeshIn->RemovePrimitiveArrangement(0);
	/////////

	// 2) polyhedron -> segmented polyhedron's vector
	std::vector<Polyhedron> polyhedron_vector;

	//double sensitivity = cos(40 * 3.14/180.0);
	QueryPerformanceFrequency(&liFrequency);
	QueryPerformanceCounter(&liCounter1);

	segmentationFromPolyhedron(polyhedron, sensitivity, polyhedron_vector);

	QueryPerformanceCounter(&liCounter2);
	//cout<<"2) Time : "<<(double)(liCounter2.QuadPart - liCounter1.QuadPart) / (double)liFrequency.QuadPart<<endl;
	std::cout << polyhedron_vector.size() << std::endl;

	//////////////////////////////////////////////////////////////
	sort(polyhedron_vector.begin(), polyhedron_vector.end(), compare);
	//////////////////////////////////////////////////////////////

	/*
	flattenPolyhedron(polyhedron_vector[0]);
	flattenPolyhedron(polyhedron_vector[3]);
	*/

	// 3) segmented polyhedron's vector -> SVXPrimitiveDataArchive's vector
	std::vector<SVXPrimitiveDataArchive> svxPrimitiveDataArchiveVector; //vtrMeshArchives
	
	// 4) 
	QueryPerformanceFrequency(&liFrequency);
	QueryPerformanceCounter(&liCounter1);
	for(std::vector<Polyhedron>::iterator itr = polyhedron_vector.begin(); itr != polyhedron_vector.end(); ++itr)
	{
		SVXPrimitiveDataArchive mesharchive;
		SVXPrimitiveBuilder::build_SVXPrimitive_from_Polyhedron(mesharchive, (*itr), 
			vxdouble3(psvxMeshArchiveInput->svxVertexBoundingOrthoBox.f3PosMinBox),
			vxdouble3(psvxMeshArchiveInput->svxVertexBoundingOrthoBox.f3PosMaxBox));
		svxPrimitiveDataArchiveVector.push_back(mesharchive);
	}
	QueryPerformanceCounter(&liCounter2);
	cout<<"4) Time : "<<(double)(liCounter2.QuadPart - liCounter1.QuadPart) / (double)liFrequency.QuadPart<<endl;

	// Register //
	vxdouble4 d4Color2 = vxdouble4(0.5, 0.5, 0.5, 1);
	psvxModuleParameter->GetCustomParameter(&d4Color2, _T("_double4_MeshColor"));
		
	for(int i = 0; i < iNumMeshes; i++)
	{
		CVXVObjectPrimitive* pCVxMeshOut = (CVXVObjectPrimitive*)vtrObjList.at(i);
		SVXPrimitiveDataArchive svxMeshArchive = svxPrimitiveDataArchiveVector.at(i);
		if(!pCVxMeshOut->RegisterPrimitiveArchive(&svxMeshArchive))
		{	
			svxMeshArchive.Delete();
			return false;
		}

		vxfloat4 f4ColorObject = f4ColorObjects[i];
		/*if(i == 1)
			f4ColorObject = vxfloat4(1.0f, 0.0f, 0.0f, 1);
		if(i == 2)
			f4ColorObject = vxfloat4(0.0f, 1.0f, 0.0f, 1);*/
		if(i == 3)
			f4ColorObject = vxfloat4(1.0f, 0.0f, 0.0f, 1);
		/*if(i == 4)
			f4ColorObject = vxfloat4(1.0f, 1.0f, 0.0f, 1);
		if(i == 5)
			f4ColorObject = vxfloat4(1.0f, 0.0f, 1.0f, 1);*/
		
		vxmatrix svxMatrixOS2VOS;
		pCVxMeshOut->InsertPrimitiveArrangement(&svxMatrixOS2VOS, &f4ColorObject);
	}
}



bool demo_step_1_segmentation_flatten(SVXModuleParameters* psvxModuleParameter)
{
	CVXVObjectPrimitive* pCVxMeshIn = (CVXVObjectPrimitive*)psvxModuleParameter->GetVXObject(vxrObjectTypePRIMITIVE, true, 0);		
	vector<CVXObject*> vtrObjList;
	int iNumMeshes = psvxModuleParameter->GetVXObjectList(&vtrObjList, vxrObjectTypePRIMITIVE, false);	
	

	// 1) input mesh -> polyhedron
	Polyhedron polyhedron;
	const SVXPrimitiveDataArchive* psvxMeshArchiveInput = pCVxMeshIn->GetPrimitiveArchive();
	PolyhedronUtils::generatePolyhedronFromSVXPrimitive(polyhedron, psvxMeshArchiveInput);
	

	// 2) polyhedron -> segmented polyhedron's vector
	std::vector<Polyhedron> polyhedron_vector;
	//double sensitivity = cos(40 * 3.14/180.0);
		segmentationFromPolyhedron(polyhedron, sensitivity, polyhedron_vector);
	std::cout << polyhedron_vector.size() << std::endl;

	//////////////////////////////////////////////////////////////
	//sort(polyhedron_vector.begin(), polyhedron_vector.end(), compare);
	//////////////////////////////////////////////////////////////

	flattenPolyhedron(polyhedron_vector[0]);
	flattenPolyhedron(polyhedron_vector[3]);
	
	// 3) segmented polyhedron's vector -> SVXPrimitiveDataArchive's vector
	std::vector<SVXPrimitiveDataArchive> svxPrimitiveDataArchiveVector; //vtrMeshArchives
	
	// 4) 
	QueryPerformanceFrequency(&liFrequency);
	QueryPerformanceCounter(&liCounter1);
	for(std::vector<Polyhedron>::iterator itr = polyhedron_vector.begin(); itr != polyhedron_vector.end(); ++itr)
	{
		SVXPrimitiveDataArchive mesharchive;
		SVXPrimitiveBuilder::build_SVXPrimitive_from_Polyhedron(mesharchive, (*itr), 
			vxdouble3(psvxMeshArchiveInput->svxVertexBoundingOrthoBox.f3PosMinBox),
			vxdouble3(psvxMeshArchiveInput->svxVertexBoundingOrthoBox.f3PosMaxBox));
		svxPrimitiveDataArchiveVector.push_back(mesharchive);
	}
	QueryPerformanceCounter(&liCounter2);
	cout<<"4) Time : "<<(double)(liCounter2.QuadPart - liCounter1.QuadPart) / (double)liFrequency.QuadPart<<endl;

	// Register //
	vxdouble4 d4Color2 = vxdouble4(0.5, 0.5, 0.5, 1);
	psvxModuleParameter->GetCustomParameter(&d4Color2, _T("_double4_MeshColor"));
		
	for(int i = 0; i < iNumMeshes; i++)
	{
		CVXVObjectPrimitive* pCVxMeshOut = (CVXVObjectPrimitive*)vtrObjList.at(i);
		SVXPrimitiveDataArchive svxMeshArchive = svxPrimitiveDataArchiveVector.at(i);
		if(!pCVxMeshOut->RegisterPrimitiveArchive(&svxMeshArchive))
		{	
			svxMeshArchive.Delete();
			return false;
		}

		vxfloat4 f4ColorObject = f4ColorObjects[i];
		/*if(i == 1)
			f4ColorObject = vxfloat4(1.0f, 0.0f, 0.0f, 1);
		if(i == 2)
			f4ColorObject = vxfloat4(0.0f, 1.0f, 0.0f, 1);*/
		if(i == 3)
			f4ColorObject = vxfloat4(1.0f, 0.0f, 0.0f, 1);
		/*if(i == 4)
			f4ColorObject = vxfloat4(1.0f, 1.0f, 0.0f, 1);
		if(i == 5)
			f4ColorObject = vxfloat4(1.0f, 0.0f, 1.0f, 1);*/
		
		vxmatrix svxMatrixOS2VOS;
		pCVxMeshOut->InsertPrimitiveArrangement(&svxMatrixOS2VOS, &f4ColorObject);
	}
}


bool demo_step_1_simplification(SVXModuleParameters* psvxModuleParameter)
{
	CVXVObjectPrimitive* pCVxMeshIn = (CVXVObjectPrimitive*)psvxModuleParameter->GetVXObject(vxrObjectTypePRIMITIVE, true, 0);		
	vector<CVXObject*> vtrObjList;
	int iNumMeshes = psvxModuleParameter->GetVXObjectList(&vtrObjList, vxrObjectTypePRIMITIVE, false);	

	// 1) input mesh -> polyhedron
	Polyhedron polyhedron;
	const SVXPrimitiveDataArchive* psvxMeshArchiveInput = pCVxMeshIn->GetPrimitiveArchive();
	PolyhedronUtils::generatePolyhedronFromSVXPrimitive(polyhedron, psvxMeshArchiveInput);

	// 2) polyhedron -> segmented polyhedron's vector
	std::vector<Polyhedron> polyhedron_vector;
	//double sensitivity = cos(40 * 3.14/180.0);
	segmentationFromPolyhedron(polyhedron, sensitivity, polyhedron_vector);
	std::cout << polyhedron_vector.size() << std::endl;

	// 2)-1 simplication 된 polyhedron에 대해서 simplification 적용 모두
	std::vector<Polyhedron> simplificated_polyhedron_vector;

	for(std::vector<Polyhedron>::iterator itr = polyhedron_vector.begin(); itr != polyhedron_vector.end(); ++itr)
	{
		PolyhedronUtils::surfaceSimplification_Constrained_Border((*itr), 0.1);
		simplificated_polyhedron_vector.push_back((*itr));
	}
	std::cout << simplificated_polyhedron_vector.size() << std::endl;

	// 3) segmented polyhedron's vector -> SVXPrimitiveDataArchive's vector
	std::vector<SVXPrimitiveDataArchive> svxPrimitiveDataArchiveVector; //vtrMeshArchives

	// 4) -1
	for(std::vector<Polyhedron>::iterator itr = simplificated_polyhedron_vector.begin(); itr != simplificated_polyhedron_vector.end(); ++itr)
	{
		SVXPrimitiveDataArchive mesharchive;
		SVXPrimitiveBuilder::build_SVXPrimitive_from_Polyhedron(mesharchive, (*itr), 
			vxdouble3(psvxMeshArchiveInput->svxVertexBoundingOrthoBox.f3PosMinBox),
			vxdouble3(psvxMeshArchiveInput->svxVertexBoundingOrthoBox.f3PosMaxBox));
		svxPrimitiveDataArchiveVector.push_back(mesharchive);
	}

	// Register //
	vxdouble4 d4Color2 = vxdouble4(0.5, 0.5, 0.5, 1);
	psvxModuleParameter->GetCustomParameter(&d4Color2, _T("_double4_MeshColor"));
		
	for(int i = 0; i < iNumMeshes; i++)
	{
		CVXVObjectPrimitive* pCVxMeshOut = (CVXVObjectPrimitive*)vtrObjList.at(i);
		SVXPrimitiveDataArchive svxMeshArchive = svxPrimitiveDataArchiveVector.at(i);
		if(!pCVxMeshOut->RegisterPrimitiveArchive(&svxMeshArchive))
		{	
			svxMeshArchive.Delete();
			return false;
		}

		vxfloat4 f4ColorObject = f4ColorObjects[i];
		vxmatrix svxMatrixOS2VOS;
		pCVxMeshOut->InsertPrimitiveArrangement(&svxMatrixOS2VOS, &f4ColorObject);
	}

	return true;
}

bool demo_step_2_split(SVXModuleParameters* psvxModuleParameter)
{
	CVXVObjectPrimitive* pCVxMeshIn = (CVXVObjectPrimitive*)psvxModuleParameter->GetVXObject(vxrObjectTypePRIMITIVE, true, 0);		
	vector<CVXObject*> vtrObjList;
	int iNumMeshes = psvxModuleParameter->GetVXObjectList(&vtrObjList, vxrObjectTypePRIMITIVE, false);	

	// 1) input mesh -> polyhedron
	Polyhedron polyhedron;
	const SVXPrimitiveDataArchive* psvxMeshArchiveInput = pCVxMeshIn->GetPrimitiveArchive();
	PolyhedronUtils::generatePolyhedronFromSVXPrimitive(polyhedron, psvxMeshArchiveInput);

	// 2) polyhedron -> segmented polyhedron's vector
	std::vector<Polyhedron> polyhedron_vector;
	//double sensitivity = cos(40 * 3.14/180.0);
	segmentationFromPolyhedron(polyhedron, sensitivity, polyhedron_vector);
	std::cout << polyhedron_vector.size() << std::endl;

	// 2)-1 simplication 된 polyhedron에 대해서 simplification 적용 모두
	std::vector<Polyhedron> simplificated_polyhedron_vector;

	for(std::vector<Polyhedron>::iterator itr = polyhedron_vector.begin(); itr != polyhedron_vector.end(); ++itr)
	{
		PolyhedronUtils::surfaceSimplification_Constrained_Border((*itr), 0.1);
		simplificated_polyhedron_vector.push_back((*itr));
	}
	std::cout << simplificated_polyhedron_vector.size() << std::endl;

	// 2)-1 simplification 된 polyhedron[6]에 split 적용후 13개의 벡터를 만듭니다.
	// 2)-2 Point_3, Point_3, Point_3 매개변수 테스트
	Point_3 p1(-0.1, 0.2, 0.0);
	Point_3 p2(1.0, 0.2, 1.0);
	Point_3 p3(-1.0, -1.0, -1.0);

	std::vector<Polyhedron> splited_polyhedron_vector;
	PolyhedronUtils::polyhedronSplit(simplificated_polyhedron_vector[7], p1, p2, p3, splited_polyhedron_vector);

	std::vector<Polyhedron> simplited_splited_polyhedron_vector;
	for (int i = 0; i< 7; i++)
	{
		simplited_splited_polyhedron_vector.push_back(simplificated_polyhedron_vector[i]);
	}
	simplited_splited_polyhedron_vector.push_back(splited_polyhedron_vector[0]);
	simplited_splited_polyhedron_vector.push_back(splited_polyhedron_vector[1]);
	for (int i = 8; i< 12; i++)
	{
		simplited_splited_polyhedron_vector.push_back(simplificated_polyhedron_vector[i]);
	}

	// 3) segmented polyhedron's vector -> SVXPrimitiveDataArchive's vector
	std::vector<SVXPrimitiveDataArchive> svxPrimitiveDataArchiveVector; //vtrMeshArchives

	// 4) -1
	for(std::vector<Polyhedron>::iterator itr = simplited_splited_polyhedron_vector.begin(); itr != simplited_splited_polyhedron_vector.end(); ++itr)
	{
		SVXPrimitiveDataArchive mesharchive;
		SVXPrimitiveBuilder::build_SVXPrimitive_from_Polyhedron(mesharchive, (*itr), 
			vxdouble3(psvxMeshArchiveInput->svxVertexBoundingOrthoBox.f3PosMinBox),
			vxdouble3(psvxMeshArchiveInput->svxVertexBoundingOrthoBox.f3PosMaxBox));
		svxPrimitiveDataArchiveVector.push_back(mesharchive);
	}

	// Register //
	vxdouble4 d4Color2 = vxdouble4(0.5, 0.5, 0.5, 1);
	psvxModuleParameter->GetCustomParameter(&d4Color2, _T("_double4_MeshColor"));
		
	for(int i = 0; i < iNumMeshes; i++)
	{
		CVXVObjectPrimitive* pCVxMeshOut = (CVXVObjectPrimitive*)vtrObjList.at(i);
		SVXPrimitiveDataArchive svxMeshArchive = svxPrimitiveDataArchiveVector.at(i);
		if(!pCVxMeshOut->RegisterPrimitiveArchive(&svxMeshArchive))
		{	
			svxMeshArchive.Delete();
			return false;
		}

		vxfloat4 f4ColorObject;
		if (i < 7)
		{
			f4ColorObject = f4ColorObjects[i];
		}
		else if(i >= 7 && i < 9) 
		{
			f4ColorObject = f4ColorObjects[i+5];
		}
		else if( i >= 9)
		{
			f4ColorObject = f4ColorObjects[i-1];
		}
		
		vxmatrix svxMatrixOS2VOS;
		pCVxMeshOut->InsertPrimitiveArrangement(&svxMatrixOS2VOS, &f4ColorObject);
	}

	return true;
}

bool demo_step_3_merge(SVXModuleParameters* psvxModuleParameter)
{
	CVXVObjectPrimitive* pCVxMeshIn = (CVXVObjectPrimitive*)psvxModuleParameter->GetVXObject(vxrObjectTypePRIMITIVE, true, 0);		
	vector<CVXObject*> vtrObjList;
	int iNumMeshes = psvxModuleParameter->GetVXObjectList(&vtrObjList, vxrObjectTypePRIMITIVE, false);	

	// 1) input mesh -> polyhedron
	Polyhedron polyhedron;
	const SVXPrimitiveDataArchive* psvxMeshArchiveInput = pCVxMeshIn->GetPrimitiveArchive();
	PolyhedronUtils::generatePolyhedronFromSVXPrimitive(polyhedron, psvxMeshArchiveInput);

	// 2) polyhedron -> segmented polyhedron's vector
	std::vector<Polyhedron> polyhedron_vector;
	//double sensitivity = cos(40 * 3.14/180.0);
	segmentationFromPolyhedron(polyhedron, sensitivity, polyhedron_vector);
	std::cout << polyhedron_vector.size() << std::endl;

	// 2)-1 simplication 된 polyhedron에 대해서 simplification 적용 모두
	std::vector<Polyhedron> simplificated_polyhedron_vector;

	for(std::vector<Polyhedron>::iterator itr = polyhedron_vector.begin(); itr != polyhedron_vector.end(); ++itr)
	{
		PolyhedronUtils::surfaceSimplification_Constrained_Border((*itr), 0.1);
		simplificated_polyhedron_vector.push_back((*itr));
	}
	std::cout << simplificated_polyhedron_vector.size() << std::endl;

	// 2)-1 simplification 된 polyhedron[6]에 split 적용후 13개의 벡터를 만듭니다.
	// 2)-2 Point_3, Point_3, Point_3 매개변수 테스트
	Point_3 p1(-0.1, 0.2, 0.0);
	Point_3 p2(1.0, 0.2, 1.0);
	Point_3 p3(-1.0, -1.0, -1.0);

	std::vector<Polyhedron> splited_polyhedron_vector;
	PolyhedronUtils::polyhedronSplit(simplificated_polyhedron_vector[7], p1, p2, p3, splited_polyhedron_vector);

	std::vector<Polyhedron> simplificated_splited_polyhedron_vector;
	for (int i = 0; i< 7; i++)
	{
		simplificated_splited_polyhedron_vector.push_back(simplificated_polyhedron_vector[i]);
	}

	simplificated_splited_polyhedron_vector.push_back(splited_polyhedron_vector[0]); // 7
	simplificated_splited_polyhedron_vector.push_back(splited_polyhedron_vector[1]); // 8


	for (int i = 8; i< 12; i++) // 9 - 12
	{
		simplificated_splited_polyhedron_vector.push_back(simplificated_polyhedron_vector[i]);
	}

	std::cout << "simplificated_splited_polyhedron_vector : " << simplificated_splited_polyhedron_vector.size() << std::endl;

	// 2)-3 splited polyhedron merge
	Polyhedron merged_polyhedron;
	PolyhedronUtils::polyhedronMerge(simplificated_splited_polyhedron_vector[7], simplificated_splited_polyhedron_vector[8], merged_polyhedron);

	std::vector<Polyhedron> merged_polyhedron_vector;

	for(int i = 0; i < 7; i++)
	{
		merged_polyhedron_vector.push_back(simplificated_polyhedron_vector[i]);
	}

	merged_polyhedron_vector.push_back(merged_polyhedron);
	
	for(int i = 8; i < 12; i++)
	{
		merged_polyhedron_vector.push_back(simplificated_polyhedron_vector[i]);
	}

	std::cout << "merged_polyhedron_vector count : " << merged_polyhedron_vector.size() << std::endl;

	// 3) segmented polyhedron's vector -> SVXPrimitiveDataArchive's vector
	std::vector<SVXPrimitiveDataArchive> svxPrimitiveDataArchiveVector; //vtrMeshArchives

	// 4) -1
	for(std::vector<Polyhedron>::iterator itr = merged_polyhedron_vector.begin(); itr != merged_polyhedron_vector.end(); ++itr)
	{
		SVXPrimitiveDataArchive mesharchive;
		SVXPrimitiveBuilder::build_SVXPrimitive_from_Polyhedron(mesharchive, (*itr), 
			vxdouble3(psvxMeshArchiveInput->svxVertexBoundingOrthoBox.f3PosMinBox),
			vxdouble3(psvxMeshArchiveInput->svxVertexBoundingOrthoBox.f3PosMaxBox));
		svxPrimitiveDataArchiveVector.push_back(mesharchive);
	}

	// Register //
	vxdouble4 d4Color2 = vxdouble4(0.5, 0.5, 0.5, 1);
	psvxModuleParameter->GetCustomParameter(&d4Color2, _T("_double4_MeshColor"));
		
	for(int i = 0; i < iNumMeshes; i++)
	{
		CVXVObjectPrimitive* pCVxMeshOut = (CVXVObjectPrimitive*)vtrObjList.at(i);
		SVXPrimitiveDataArchive svxMeshArchive = svxPrimitiveDataArchiveVector.at(i);
		if(!pCVxMeshOut->RegisterPrimitiveArchive(&svxMeshArchive))
		{	
			svxMeshArchive.Delete();
			return false;
		}

		vxfloat4 f4ColorObject = f4ColorObjects[i];
		vxmatrix svxMatrixOS2VOS;
		pCVxMeshOut->InsertPrimitiveArrangement(&svxMatrixOS2VOS, &f4ColorObject);
	}

	return true;
}

bool _function_launcher_SSU(SVXModuleParameters* psvxModuleParameter, SVXLocalProgress* psvxProgress)
{
	std::cout << "[@ _function_launcher_SSU Start @]" << std::endl;

	// GET_REQUIRED_MESH_COUNT_FEATURE_BASED_SIMPLIFICATION
	// FILL_MESHES_WITH_FEATURE_BASED_SIMPLIFICATION
	// FILL_MESHES_DIVIDED_BY_PLANE
	wstring strFunctionModeInLauncher = _T("NONE");
	psvxModuleParameter->GetCustomParameter(&strFunctionModeInLauncher, _T("_string_FunctionModeInLauncher"));

	//────────────────────────────────────────────────────────────────────────────────────────────────//
	if(strFunctionModeInLauncher.compare(_T("GET_REQUIRED_MESH_COUNT_FEATURE_BASED_SIMPLIFICATION")) == 0)
	{		
		std::cout << "[ _function_launcher_SSU Start in if statement ] " << std::endl;
		
		CVXVObjectPrimitive* pCVxMeshIn = (CVXVObjectPrimitive*)psvxModuleParameter->GetVXObject(vxrObjectTypePRIMITIVE, true, 0);
		CVXLObject* pCLObject = (CVXLObject*)psvxModuleParameter->GetVXObject(vxrObjectTypeCUSTOMLIST, false, 0);
		const SVXPrimitiveDataArchive* psvxMeshArchiveInput = pCVxMeshIn->GetPrimitiveArchive();

		vector<double> vtrResultContainer;
		vtrResultContainer.push_back((double)iNumRequiredMeshes);
		pCLObject->RegisterList(_T("_vlist_DOUBLE_CustomResultOutFromLauncherSSU"), &vtrResultContainer);
	
		std::cout << "[ _function_launcher_SSU End in if statement ] " << std::endl;
		return true;
	}
	//────────────────────────────────────────────────────────────────────────────────────────────────//
	else if(strFunctionModeInLauncher.compare(_T("FILL_MESHES_WITH_FEATURE_BASED_SIMPLIFICATION")) == 0)
	{	
		std::cout << "[ _function_launcher_SSU Start in else if statement ] " << std::endl;
		
		bool rb = false;
		switch (demo_step)
		{
		case 0: // segmentation
			rb = demo_step_0_segmentation(psvxModuleParameter);
			if(rb) {
				demo_step += 1;
				//iNumRequiredMeshes = 12;
			}
			break;
		case 1: // simplification
			rb = demo_step_1_segmentation_flatten(psvxModuleParameter);
			if (rb) {
				demo_step += 1;
				//iNumRequiredMeshes = 13;
			}
			break; 
		case 2: // split
			rb = demo_step_2_split(psvxModuleParameter);
			if (rb) {
				demo_step += 1;
				iNumRequiredMeshes = 12;
			}
			break;
		case 3: // merge
			rb = demo_step_3_merge(psvxModuleParameter);
			if (rb) {
				demo_step = 0;
				iNumRequiredMeshes = 12;
			}
			break;
		}
		
		std::cout << "[ _function_launcher_SSU End return true in else if statement ] " << std::endl;
		return true;
	}
	//────────────────────────────────────────────────────────────────────────────────────────────────//
	else if(strFunctionModeInLauncher.compare(_T("FILL_MESHES_DIVIDED_BY_PLANE")) == 0)
	{
		return true;
	}
	else return false;

	std::cout << "[@ _function_launcher_SSU End @]" << std::endl;
	return true;
}

void segmentationFromPolyhedron(Polyhedron & poly, double sensitivity, std::vector<Polyhedron> & polyvector)
{
	// Facet Normal 계산
	std::for_each(poly.facets_begin(), poly.facets_end(), Facet_normal());
	
	// Facet Id 추가 테스트
	int id = 1;
    for(Facet_iterator fit = poly.facets_begin(); fit != poly.facets_end(); ++fit)
    {
        fit->setFid(id);
        ++id;
    }
	// segment's facet_handle group vector
	std::vector< std::vector<Polyhedron::Facet_handle> > segments;
    
	// polyhedron에 대한 segmentation 진행
    for (Facet_iterator fit = poly.facets_begin(); fit != poly.facets_end(); ++fit) {
        
        Polyhedron::Halfedge_around_facet_circulator facet_circulator = fit->facet_begin();
        
		if(!facet_circulator->facet()->is_visited())
		{
			std::vector<Polyhedron::Facet_handle> facet_group;
			segments.push_back(facet_group);
			vxfloat3 average_normal;
			average_normal.x = facet_circulator->facet()->normal().x();        
			average_normal.y = facet_circulator->facet()->normal().y();        
			average_normal.z = facet_circulator->facet()->normal().z();        
			PolyhedronUtils::segmentAroundAFacet(segments[segments.size()-1], facet_circulator, sensitivity, average_normal, 1);
		}
    }

	// segment's group 에서 각 segments에 대한 polyhedron vector container를 생성
	std::vector< std::vector<Polyhedron::Facet_handle> >::const_iterator cfit = segments.cbegin();
	for (; cfit != segments.cend(); ++cfit) {
        Polyhedron p;
        PolyhedronUtils::generatePolyhedronFromFacetsVector(p, (*cfit));
        polyvector.push_back(p);
    }
}

