#include "PolyhedronUtils.h"
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <hash_map>
#include <unordered_map>
#include <CGAL/centroid.h>
#include <CGAL/bounding_box.h>
#include <CGAL/linear_least_squares_fitting_3.h>

//────────────────────────────────────────────────────//
template <class HDS>
class BuildFromSVXPrimitve : public CGAL::Modifier_base<HDS> 
{
private:
	const std::vector<Point_3>& _vertices;
	const std::vector<boost::tuple<int,int,int>> _facets;

public:
	BuildFromSVXPrimitve(const std::vector<Point_3>& vertices, 
				 const std::vector<boost::tuple<int, int, int>>& facets)
		: _vertices(vertices), _facets(facets)
	{ }

	void operator()(HDS& hds)
	{
		CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
	
		B.begin_surface(_vertices.size(), _facets.size(), 0);

		// vertex 추가	
		for(uint i = 0; i < _vertices.size(); i++)
			B.add_vertex(_vertices[i]);
		
		// face 추가	
		for(uint i = 0; i < _facets.size(); i++)
		{
			B.begin_facet();
				B.add_vertex_to_facet(_facets[i].template get<0>());
				B.add_vertex_to_facet(_facets[i].template get<1>());
				B.add_vertex_to_facet(_facets[i].template get<2>());
			B.end_facet();
		}
		
		B.end_surface();
	}
};

#define THRESHOLD 0.0000001
//────────────────────────────────────────────────────//
struct eqPoint_3
{
  bool operator()(Point_3 s1, Point_3 s2) const
  {
	  return (s1.x()-s2.x())*(s1.x()-s2.x())+(s1.y()-s2.y())*(s1.y()-s2.y())+(s1.z()-s2.z())*(s1.z()-s2.z()) < THRESHOLD;
  }
};

namespace std
{
    template<>
    struct hash<Point_3>
    {
	public:
		typedef std::size_t result_type;
		result_type operator()(Point_3 const& s) const
        {
            result_type const h1 ( std::hash<double>()(s.x()) );
            result_type const h2 ( std::hash<double>()(s.y()) );
			result_type const h3 ( std::hash<double>()(s.z()) );
            return h1 ^ (h2 << 1)^(h3<<2);
        }
    };
}


void PolyhedronUtils::generatePolyhedronFromSVXPrimitive(Polyhedron& p, const SVXPrimitiveDataArchive* svx)
{
	// SVXPrimitiveDataArchive로부터 버텍스 가져오기
	map<EnumVXRVertexType, vxfloat3*> inputVertex_map(svx->mapVerticeDefinitions);
	vxfloat3* f3Vertices = inputVertex_map.find(vxrVertexTypePOSITION)->second;

	// 버텍스들로부터 버텍스, 인덱스 재구성하기 (버텍스가 share 되도록하는게 목적)
	std::vector<Point_3>					 vertices;
	std::vector<boost::tuple<int, int, int>> facets;

	/*hash< Point_3> a;
	int b = a(Point_3(0., 0., 0.));
	*/
	
	//hash_map<Point_3, int, hash<Point_3>, eqPoint_3>	vertex_map;
	map<Point_3, int>	vertex_map;
	//unordered_map<Point_3, int, hash<Point_3>, eqPoint_3>	vertex_map;
	Point_3					point;
	int						ijk[3];
	int						count = 0;
	int						index = 0;

	for(int i = 0; i < svx->uiNumPolygons; i++)
	{
		for(int vtx = 0; vtx < 3; vtx++)
		{
			index = (i * 3) + vtx; // (0, 1, 2), (3, 4, 5), (6, 7, 8), ...
			point = Point_3(f3Vertices[index].x, f3Vertices[index].y, f3Vertices[index].z);			

			if(vertex_map.find(point) == vertex_map.end()) // 아직 추가되지 않은 버텍스라면 추가함
			{
				ijk[vtx] = count;
				vertex_map[point] = count;
				vertices.push_back(point);
				count++;
				
			} 
			else 
			{
				ijk[vtx] = vertex_map[point];
			}
		}

		facets.push_back(boost::make_tuple(ijk[0], ijk[1], ijk[2]));
	}
	// Polyhedron 만들기
	BuildFromSVXPrimitve<Polyhedron::HalfedgeDS> svx_builder(vertices, facets);
	p.delegate(svx_builder);
}

//----------------------------------------------------------------------------------------------------//
template <class HDS>
class BuildFromFacetsVector : public CGAL::Modifier_base<HDS>
{
private:
	const std::vector<Point_3>& _vertices;
	const std::vector<boost::tuple<int,int,int>> _facets;
    
    typedef std::vector<Point_3>::const_iterator point_iter;
    typedef std::vector<boost::tuple<int, int, int>>::const_iterator facet_iter;
public:
	BuildFromFacetsVector(const std::vector<Point_3>& vertices,
                         const std::vector<boost::tuple<int, int, int>>& facets)
    : _vertices(vertices), _facets(facets)
	{ }
    
	void operator()(HDS& hds)
	{
		CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
        
		B.begin_surface(_vertices.size(), _facets.size(), _facets.size()*3);
        
		// vertex 추가
		for(point_iter pit = _vertices.begin(); pit != _vertices.end(); ++pit)
			B.add_vertex((*pit));
		
		// face 추가
		for(facet_iter fit = _facets.begin(); fit != _facets.end(); ++fit)
		{
			B.begin_facet();
            B.add_vertex_to_facet((*fit).get<0>());
            B.add_vertex_to_facet((*fit).get<1>());
            B.add_vertex_to_facet((*fit).get<2>());
			B.end_facet();
		}
		
		B.end_surface();
	}
};
//-----------------------------------------------------------------------------------------------------//

void PolyhedronUtils::generatePolyhedronFromFacetsVector(Polyhedron & P, const std::vector<Polyhedron::Facet_handle> &facet_group)
{
    std::vector<Point_3> vertices;
    std::vector<boost::tuple<int, int, int>> facets;
    
    //std::map<Point_3, int> vertex_map;
	unordered_map<Point_3, int, hash<Point_3>, eqPoint_3> vertex_map;
    
    std::vector<Polyhedron::Facet_handle>::const_iterator cit1 = facet_group.cbegin();
    
    int vertex_index = 0;
    
    for (; cit1 != facet_group.cend(); ++cit1) {
        
        Point_3 p1 = (*cit1)->halfedge()->vertex()->point();
        Point_3 p2 = (*cit1)->halfedge()->next()->vertex()->point();
        Point_3 p3 = (*cit1)->halfedge()->next()->next()->vertex()->point();
        
        if (vertex_map.empty()) {
            vertex_map[p1] = vertex_index++;
            vertices.push_back(p1);
        }
        else
        {
            if (vertex_map.find(p1) == vertex_map.end()) {
                vertex_map[p1] = vertex_index++;
                vertices.push_back(p1);
            }
        }
        
        if (vertex_map.find(p2) == vertex_map.end()) {
            vertex_map[p2] = vertex_index++;
            vertices.push_back(p2);
        }
        
        if (vertex_map.find(p3) == vertex_map.end()) {
            vertex_map[p3] = vertex_index++;
            vertices.push_back(p3);
        }
    }
    

    Point_3 point;
    int ijk[3];
    int index = 0;
    
    std::vector<Polyhedron::Facet_handle>::const_iterator cit2 = facet_group.cbegin();
    
    for (; cit2 != facet_group.end(); ++cit2) {
        Point_3 p1 = (*cit2)->halfedge()->vertex()->point();
        Point_3 p2 = (*cit2)->halfedge()->next()->vertex()->point();
        Point_3 p3 = (*cit2)->halfedge()->next()->next()->vertex()->point();
        
        index = vertex_map.find(p1)->second;
        ijk[0] = index;
        
        index = vertex_map.find(p2)->second;
        ijk[1] = index;
        
        index = vertex_map.find(p3)->second;
        ijk[2] = index;
        
        facets.push_back(boost::make_tuple(ijk[0], ijk[1], ijk[2]));
    }
    
    
    BuildFromFacetsVector<Polyhedron::HalfedgeDS> facet_builder(vertices, facets);
    P.delegate(facet_builder);
    
}

void PolyhedronUtils::segmentAroundAFacet(std::vector<Polyhedron::Facet_handle> & facet_group, const Polyhedron::Halfedge_around_facet_circulator facet_circulator, double sensitivity)
{
	Polyhedron::Facet_handle center_facet_handle = facet_circulator->facet();
            
	center_facet_handle->setVisited(true);
    facet_group.push_back(center_facet_handle);

	Polyhedron::Halfedge_handle currEdge;
	int i;
	currEdge = facet_circulator->facet_begin();
	for ( i=0; i < facet_circulator->facet_degree() ; currEdge = currEdge->next(), i++){
	        
		if (!currEdge->opposite()->facet_begin()->facet()->is_visited()) {
                
			if ((center_facet_handle->normal() * currEdge->opposite()->facet_begin()->facet()->normal()) > sensitivity) {
				segmentAroundAFacet(facet_group, currEdge->opposite()->facet_begin(), sensitivity);
			}     

		}
	}
}

/*
void PolyhedronUtils::segmentAroundAFacet(std::vector< std::vector<Polyhedron::Facet_handle> > & segments, const Polyhedron::Halfedge_around_facet_circulator facet_circulator, double sensitivity)
{
    if (!facet_circulator->facet()->is_visited()) {
            
        Polyhedron::Facet_handle center_facet_handle = facet_circulator->facet();
            
        std::vector<Polyhedron::Facet_handle> facet_group;
            
        center_facet_handle->setVisited(true);
        facet_group.push_back(center_facet_handle);
            
        segments.push_back(facet_group);
            
        unsigned long currentSegmentationKey = segments.size()-1;
            
        if (!facet_circulator->opposite()->facet_begin()->facet()->is_visited()) {
                
            if ((center_facet_handle->normal() * facet_circulator->opposite()->facet_begin()->facet()->normal()) > sensitivity) {
                facet_circulator->opposite()->facet_begin()->facet()->setVisited(true);
                segments[currentSegmentationKey].push_back(facet_circulator->opposite()->facet_begin()->facet());
                    
                segmentAroundAFacet(segments, facet_circulator->opposite()->facet_begin(), sensitivity);
            }     

        }
            
        if (!facet_circulator->next()->opposite()->facet_begin()->facet()->is_visited()) {
                
            if ((center_facet_handle->normal() * facet_circulator->next()->opposite()->facet_begin()->facet()->normal()) > sensitivity) {
                facet_circulator->next()->opposite()->facet_begin()->facet()->setVisited(true);
                segments[currentSegmentationKey].push_back(facet_circulator->next()->opposite()->facet_begin()->facet());
                    
                segmentAroundAFacet(segments, facet_circulator->next()->opposite()->facet_begin(), sensitivity);
            }

        }
            
        if (!facet_circulator->next()->next()->opposite()->facet_begin()->facet()->is_visited()) {
                
            if ((center_facet_handle->normal() * facet_circulator->next()->next()->opposite()->facet_begin()->facet()->normal()) > sensitivity) {
                facet_circulator->next()->next()->opposite()->facet_begin()->facet()->setVisited(true);
                segments[currentSegmentationKey].push_back(facet_circulator->next()->next()->opposite()->facet_begin()->facet());
                    
                segmentAroundAFacet(segments, facet_circulator->next()->next()->opposite()->facet_begin(), sensitivity);
            }

        }
            
    }
    else
    {
        unsigned long lastSegmentationKey = segments.size()-1;
        Polyhedron::Facet_handle center_facet_handle = facet_circulator->facet();
            
        if (!facet_circulator->opposite()->facet_begin()->facet()->is_visited()) {
                
            if ((center_facet_handle->normal() * facet_circulator->opposite()->facet_begin()->facet()->normal()) > sensitivity) {
                facet_circulator->opposite()->facet_begin()->facet()->setVisited(true);
                segments[lastSegmentationKey].push_back(facet_circulator->opposite()->facet_begin()->facet());
                    
                segmentAroundAFacet(segments, facet_circulator->opposite()->facet_begin(), sensitivity);
            }
                
        }
            
        if (!facet_circulator->next()->opposite()->facet_begin()->facet()->is_visited()) {
                
            if ((center_facet_handle->normal() * facet_circulator->next()->opposite()->facet_begin()->facet()->normal()) > sensitivity) {
                facet_circulator->next()->opposite()->facet_begin()->facet()->setVisited(true);
                segments[lastSegmentationKey].push_back(facet_circulator->next()->opposite()->facet_begin()->facet());
                    
                segmentAroundAFacet(segments, facet_circulator->next()->opposite()->facet_begin(), sensitivity);
            }
        }
            
        if (!facet_circulator->next()->next()->opposite()->facet_begin()->facet()->is_visited()) {
                
            if ((center_facet_handle->normal() * facet_circulator->next()->next()->opposite()->facet_begin()->facet()->normal()) > sensitivity) {
                facet_circulator->next()->next()->opposite()->facet_begin()->facet()->setVisited(true);
                segments[lastSegmentationKey].push_back(facet_circulator->next()->next()->opposite()->facet_begin()->facet());
                    
                segmentAroundAFacet(segments, facet_circulator->next()->next()->opposite()->facet_begin(), sensitivity);
                    
            }
        }
    }
}
*/
void PolyhedronUtils::surfaceSimplification_Constrained_Border(Polyhedron & p, double stopCondition)
{
	// map used to check that constrained_edges and the points of its vertices
	// are preserved at the end of the simplification
	std::map<Polyhedron::Halfedge_handle,std::pair<Point_3, Point_3> >constrained_edges;
	//hash_map<Polyhedron::Halfedge_handle,std::pair<Point_3, Point_3> >constrained_edges;
	std::size_t nb_border_edges=0;

	for (Polyhedron::Halfedge_iterator hit=p.halfedges_begin(),
										hit_end=p.halfedges_end();
										hit!=hit_end; ++hit )
	{
		if ( hit->is_border() ){
			constrained_edges[hit]=std::make_pair( hit->opposite()->vertex()->point(),
													hit->vertex()->point() );
			++nb_border_edges;
		}
	}

	// Contract the surface mesh as much as possible
	//SMS::Count_stop_predicate<Polyhedron> stop(stopCondition);
	
	SMS::Count_ratio_stop_predicate<Polyhedron> stop(stopCondition);
	// This the actual call to the simplification algorithm.
	// The surface mesh and stop conditions are mandatory arguments.
	// The index maps are needed because the vertices and edges
	// of this surface mesh lack an "id()" field.
	//int r = SMS::edge_collapse
	//		(p
	//		,stop
	//		,CGAL::vertex_index_map(boost::get(CGAL::vertex_external_index,p))
	//				.edge_index_map  (boost::get(CGAL::edge_external_index  ,p))
	//				.edge_is_constrained_map(Border_is_constrained_edge_map())
	//				.get_placement(Placement())
	//		);

	int r = SMS::edge_collapse
            (p
            ,stop
            ,CGAL::vertex_index_map(get(CGAL::vertex_external_index,p))
                  .halfedge_index_map  (get(CGAL::halfedge_external_index  ,p))
                  .edge_is_constrained_map(Border_is_constrained_edge_map(p))
                  .get_placement(Placement())
            );
	// now check!
	for (Polyhedron::Halfedge_iterator hit=p.halfedges_begin(),
										hit_end=p.halfedges_end();
										hit!=hit_end; ++hit )
	{
		if (hit->is_border())
		{
			--nb_border_edges;
			assert( constrained_edges[hit] ==
					std::make_pair( hit->opposite()->vertex()->point(),
									hit->vertex()->point() ) );
		}
	}
	assert( nb_border_edges==0 );
}

void PolyhedronUtils::surfaceSimplification_Edge_Collapse(Polyhedron & p, double stopCondition)
{
	SMS::Count_ratio_stop_predicate<Polyhedron> stop(stopCondition);
     
  // This the actual call to the simplification algorithm.
  // The surface mesh and stop conditions are mandatory arguments.
  // The index maps are needed because the vertices and edges
  // of this surface mesh lack an "id()" field.
  int r = SMS::edge_collapse
            (p
            ,stop
            ,CGAL::vertex_index_map(get(CGAL::vertex_external_index,p)) 
             .halfedge_index_map  (get(CGAL::halfedge_external_index  ,p)) 
             .get_cost (SMS::Edge_length_cost <Polyhedron>())
             .get_placement(SMS::Midpoint_placement<Polyhedron>())
            );
}

void PolyhedronUtils::polyhedronMerge(Polyhedron & p1, Polyhedron & p2, Polyhedron & meged_polyhedron)
{
	std::vector<Polyhedron::Facet_handle> facets_vector;

	Facet_iterator p1_fit = p1.facets_begin();

	for(; p1_fit != p1.facets_end(); ++p1_fit)
	{
		facets_vector.push_back(p1_fit);
	}

	Facet_iterator p2_fit = p2.facets_begin();

	for(; p2_fit != p2.facets_end(); ++p2_fit)
	{
		facets_vector.push_back(p2_fit);
	}

	generatePolyhedronFromFacetsVector(meged_polyhedron, facets_vector);
}

void PolyhedronUtils::polyhedronSplit(Polyhedron & poly, Point_3 & v1, Point_3 & v2, Point_3 & v3, std::vector<Polyhedron> & splited_polyhedron_vector)
{
	std::map<Polyhedron::Facet_handle, Point_3> center_of_facet_map;

	// 1) facet별 중점 계산
	Facet_iterator fit = poly.facets_begin();
	for(; fit != poly.facets_end(); ++fit)
	{
		Point_3 p1 = fit->halfedge()->vertex()->point();
		Point_3 p2 = fit->halfedge()->next()->vertex()->point();
		Point_3 p3 = fit->halfedge()->next()->next()->vertex()->point();

		center_of_facet_map[fit] = Point_3((p1.x() + p2.x() + p3.x()) / 3, (p1.y() + p2.y() + p3.y()) / 3, (p1.z() + p2.z() + p3.z()) / 3);
	}
	// 2) Plane_3 생성
	Plane_3 plane(v1, v2, v3);


	// 3) Plane_3 기준으로 facets Grouping
	std::vector<Polyhedron::Facet_handle> negative_group;
	std::vector<Polyhedron::Facet_handle> positive_group;

	std::map<Polyhedron::Facet_handle, Point_3>::iterator mit = center_of_facet_map.begin();
	for(; mit != center_of_facet_map.end(); ++mit)
	{
		Point_3 point = mit->second;
		if(plane.has_on_negative_side(point))
		{
			negative_group.push_back(mit->first);
		}
		else
		{
			positive_group.push_back(mit->first);
		}
	}

	// 4) group별 Polyhedron 생성
	Polyhedron negative_polyhedron;
	Polyhedron positive_polyhedron;

	generatePolyhedronFromFacetsVector(negative_polyhedron, negative_group);
	generatePolyhedronFromFacetsVector(positive_polyhedron, positive_group);

	splited_polyhedron_vector.push_back(negative_polyhedron);
	splited_polyhedron_vector.push_back(positive_polyhedron);
	
}

void PolyhedronUtils::polyhedronSplit(Polyhedron & poly, Plane_3 & plane, std::vector<Polyhedron> & splited_polyhedron_vector)
{
	std::map<Polyhedron::Facet_handle, Point_3> center_of_facet_map;

	// 1) facet별 중점 계산
	Facet_iterator fit = poly.facets_begin();
	for(; fit != poly.facets_end(); ++fit)
	{
		Point_3 p1 = fit->halfedge()->vertex()->point();
		Point_3 p2 = fit->halfedge()->next()->vertex()->point();
		Point_3 p3 = fit->halfedge()->next()->next()->vertex()->point();

		center_of_facet_map[fit] = Point_3((p1.x() + p2.x() + p3.x()) / 3, (p1.y() + p2.y() + p3.y()) / 3, (p1.z() + p2.z() + p3.z()) / 3);
	}

	std::cout << "Input Polyhedron Facet Count : " << poly.size_of_facets() << std::endl;
	std::cout << "Center of Facet Map Count : " << center_of_facet_map.size() << std::endl;

	// 2) Plane_3 생성
	// 입력 매개변수 plane으로 대체
	std::cout << "Plane : " << plane << std::endl;
		
	std::cout << "Plane's Orthogonal Direction : " << plane.orthogonal_direction() << std::endl;
	std::cout << "Plane's Orthogonal Vector : " << plane.orthogonal_vector() << std::endl;

	// 3) Plane_3 기준으로 facets Grouping
	std::vector<Polyhedron::Facet_handle> negative_group;
	std::vector<Polyhedron::Facet_handle> positive_group;

	std::map<Polyhedron::Facet_handle, Point_3>::iterator mit = center_of_facet_map.begin();
	for(; mit != center_of_facet_map.end(); ++mit)
	{
		Point_3 point = mit->second;
		
		
		if(plane.has_on_positive_side(point))
		{
			positive_group.push_back(mit->first);
		}
		else
		{
			negative_group.push_back(mit->first);
		}
	}

	// 4) group별 Polyhedron 생성
	Polyhedron negative_polyhedron;
	Polyhedron positive_polyhedron;

	if(negative_group.size() != 0) {
		generatePolyhedronFromFacetsVector(negative_polyhedron, negative_group);
	}
	std::cout << "Negative Polyhedron : " << negative_group.size() << std::endl;
	if(positive_group.size() != 0) {
		generatePolyhedronFromFacetsVector(positive_polyhedron, positive_group);
	}
	std::cout << "Positive Polyhedron : " << positive_group.size() << std::endl;

	if(negative_group.size() != 0) {
		splited_polyhedron_vector.push_back(negative_polyhedron);
	}

	if(positive_group.size() != 0) {
		splited_polyhedron_vector.push_back(positive_polyhedron);
	}
}

void PolyhedronUtils::make_pca_plane_3(Polyhedron & poly, Plane_3 & pca_plane)
{
	// get triangles from the mesh
  std::list<Triangle_3> triangles;
  Polyhedron::Facet_iterator f;
  for(f = poly.facets_begin();
    f != poly.facets_end();
    ++f)
  {
    const Point_3& a = f->halfedge()->vertex()->point();
    const Point_3& b = f->halfedge()->next()->vertex()->point();
    const Point_3& c = f->halfedge()->prev()->vertex()->point();
    triangles.push_back(Triangle_3(a,b,c));
  }

  // fit line to triangles
  Line_3 line;
  std::cout << "Fit line...";
  CGAL::linear_least_squares_fitting_3(triangles.begin(),triangles.end(),line,CGAL::Dimension_tag<2>());
  std::cout << "ok" << std::endl;

  // compute centroid
  Point_3 center_of_mass = CGAL::centroid(triangles.begin(),triangles.end());

  // compute bounding box diagonal
  Iso_cuboid_3 bbox = CGAL::bounding_box(poly.points_begin(),poly.points_end());

  // compute scale for rendering using diagonal of bbox
  Point_3 cmin = (bbox.min)();
  Point_3 cmax = (bbox.max)();
  FT diag = std::sqrt(CGAL::squared_distance(cmin,cmax));

  // construct a 3D bar
  Vector_3 u = line.to_vector();
  u = u / std::sqrt(u*u);

  Point_3 a = center_of_mass + u * diag;
  Point_3 b = center_of_mass - u * diag;

  pca_plane = line.perpendicular_plane(a);
}




