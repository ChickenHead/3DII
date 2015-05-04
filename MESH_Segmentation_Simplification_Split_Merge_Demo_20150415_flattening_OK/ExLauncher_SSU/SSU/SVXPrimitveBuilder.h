#pragma once

#include "VXEngineGlobalUnit.h"

class SVXPrimitiveBuilder
{
public:
	template<typename T>
	static void build_SVXPrimitive_from_Polyhedron(SVXPrimitiveDataArchive& svx, T& poly,
												   vxdouble3 d3PosPhsMin, vxdouble3 d3PosPhsMax);
};

template<typename T>
void SVXPrimitiveBuilder::build_SVXPrimitive_from_Polyhedron(SVXPrimitiveDataArchive& svx, T& poly,
															 vxdouble3 d3PosPhsMin, vxdouble3 d3PosPhsMax)
{
	svx.Delete();

	// vertex 관련
	svx.uiNumVertice = (uint)poly.size_of_vertices();
	vxfloat3* vertices = new vxfloat3[svx.uiNumVertice];

	int i = 0;
	for(typename T::Vertex_iterator v = poly.vertices_begin(); v != poly.vertices_end(); ++v)
	{
		vertices[i++] = vxfloat3(v->point().x(), v->point().y(), v->point().z());
	}

	svx.ReplaceOrAddVerticeDefinition(vxrVertexTypePOSITION, vertices);	

	// normal
	vxfloat3* normal = new vxfloat3[svx.uiNumVertice];
	typename T::Halfedge_around_vertex_circulator he;

	i = 0;
	float nx, ny, nz;
	Vector_3 u, v;
	for(typename T::Vertex_iterator vtx = poly.vertices_begin(); vtx != poly.vertices_end(); ++vtx)
	{
		he = vtx->vertex_begin();

		u = (he->next()->vertex()->point()) - (he->vertex()->point());
		v = (he->next()->next()->vertex()->point()) - (he->vertex()->point());

		nx = u.y() * v.z() - u.z() * v.y();
		ny = u.z() * v.x() - u.x() * v.z();
		nz = u.x() * v.y() - u.y() * v.x();

		vxfloat3 nor = vxfloat3(nx, ny, nz);
		VXHMNormalizeVector(&nor, &nor);
		normal[i++] = nor;
	}
	svx.ReplaceOrAddVerticeDefinition(vxrVertexTypeNORMAL, normal);


	// index 관련
	svx.uiNumIndice = uint(3 * poly.size_of_facets());
	uint* indices = new uint[svx.uiNumIndice];

	int row = 0;
	int column = 0;
	typename T::Halfedge_around_facet_circulator facet_cir;
	for(typename T::Facet_iterator f = poly.facets_begin(); f != poly.facets_end(); ++f)
	{
		facet_cir = f->facet_begin();

		if(CGAL::circulator_size(facet_cir) == 3) 
		{
			column = 0;
			do
			{
				indices[row*3+column] = std::distance(poly.vertices_begin(), facet_cir->vertex());
				++column;
			} while(++facet_cir != f->facet_begin());
		}
		else
		{ }
		++row;
	}

	svx.uiStrideIndexListPerPolygon = 3;
	svx.uiNumPolygons = poly.size_of_facets();
	svx.puiIndexList = indices;

	/*
	///////////////////////////////////////////////////////////////////////////////////////
	// normal vector 관련
	vxfloat3* normal = new vxfloat3[svx.uiNumVertice];
	typename T::Halfedge_around_vertex_circulator	he;
	typename T::Halfedge_around_vertex_circulator	end;
	typename T::Face_handle							fh;
	for(typename T::Vertex_iterator v = poly.vertices_begin(); v != poly.vertices_end(); ++v)
	{
		he = v->vertex_begin();
		end = he;
		
		CGAL_For_all(he, end)
		{
			fh = he->facet();
			fh->face_begin();
		}
	}
	svx.ReplaceOrAddVerticeDefinition(vxrVertexTypeNORMAL, normal);
	///////////////////////////////////////////////////////////////////////////////////////
	*/

	// bounding box 관련	
	svx.svxVertexBoundingOrthoBox.f3PosMinBox = vxfloat3(d3PosPhsMin);
	svx.svxVertexBoundingOrthoBox.f3PosMaxBox = vxfloat3(d3PosPhsMax);

	// 기타
	svx.bIsPrimitiveFrontCCW = false;
	svx.bIsPrimitiveStripe = false;
	svx.ePrimitiveType = vxrPrimitiveTypeTRIANGLE;
}