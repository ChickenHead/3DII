#pragma once

#include "VXEngineGlobalUnit.h"

#include "cglab_facet.h"

#include <boost/tuple/tuple.hpp>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>

//-- Simplification Header Part Start--
// Adaptor for Polyhedron_3
#include <CGAL/Surface_mesh_simplification/HalfedgeGraph_Polyhedron_3.h>

// Simplification function
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>

// Midpoint placement policy
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>

//Placement wrapper
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Constrained_placement.h>

// Stop-condition policy
//#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_stop_predicate.h>

// Stop-condition policy
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_predicate.h>

// Stop-condition policy
//#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_length_cost.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>

namespace SMS = CGAL::Surface_mesh_simplification ;
//-- Simplification Header Part End--

typedef CGAL::Simple_cartesian<double> Kernel;
typedef CGAL::Polyhedron_3<Kernel, CGLab_items> Polyhedron;
typedef Polyhedron::Facet_iterator Facet_iterator;

typedef Kernel::Point_3 Point_3;
typedef Kernel::Vector_3 Vector_3;
typedef Kernel::Plane_3 Plane_3;

typedef Kernel::Iso_cuboid_3 Iso_cuboid_3;
typedef Kernel::Triangle_3 Triangle_3;
typedef Kernel::Line_3 Line_3;
typedef Kernel::FT FT;

//-- Simplification Header Part Start--
//
// BGL property map which indicates whether an edge is marked as non-removable
//
//struct Border_is_constrained_edge_map{
//  typedef boost::graph_traits<Polyhedron>::edge_descriptor key_type;
//  typedef bool value_type;
//  typedef value_type reference;
//  typedef boost::readable_property_map_tag category;
//  friend bool get(Border_is_constrained_edge_map, key_type edge) {
//    return edge->is_border_edge();
//  }
//};

//
// Placement class
//
//typedef SMS::Constrained_placement<SMS::Midpoint_placement<Polyhedron>,
//                                   Border_is_constrained_edge_map > Placement;

// 20150114 CGAL 4.5.1 Mesh Simplification Example 소스 변경에 따른 수정
//
// BGL property map which indicates whether an edge is marked as non-removable
//
struct Border_is_constrained_edge_map{
  const Polyhedron* sm_ptr;
  typedef boost::graph_traits<Polyhedron>::edge_descriptor key_type;
  typedef bool value_type;
  typedef value_type reference;
  typedef boost::readable_property_map_tag category;

  Border_is_constrained_edge_map()
  {}

  Border_is_constrained_edge_map(const Polyhedron& sm)
    : sm_ptr(&sm)
  {}

  friend bool get(Border_is_constrained_edge_map m, const key_type& edge) {
    return CGAL::is_border(edge, *m.sm_ptr);
  }
};

//
// Placement class
//
typedef SMS::Constrained_placement<SMS::Midpoint_placement<Polyhedron>,
                                   Border_is_constrained_edge_map > Placement;

//-- Simplification Header Part End--

//#include <CGAL/Homogeneous.h>
//#include <CGAL/Polyhedron_traits_with_normals_3.h>
//#include <CGAL/Polyhedron_3.h>
//
//typedef CGAL::Homogeneous<int>                         Kernel;
//typedef Kernel::Point_3                                Point_3;
//typedef Kernel::Vector_3                               Vector_3;
//typedef CGAL::Polyhedron_traits_with_normals_3<Kernel> Traits;
//typedef CGAL::Polyhedron_3<Traits>                     Polyhedron;

class PolyhedronUtils
{
public:
	static void generatePolyhedronFromSVXPrimitive(Polyhedron& p, const SVXPrimitiveDataArchive* svx);
	static void generatePolyhedronFromFacetsVector(Polyhedron & P, const std::vector<Polyhedron::Facet_handle> &facet_group);
	static void segmentAroundAFacet(/*std::vector< std::vector<Polyhedron::Facet_handle> >*/std::vector<Polyhedron::Facet_handle> & segments, const Polyhedron::Halfedge_around_facet_circulator facet_circulator, double sensitivity);
	
	static void surfaceSimplification_Constrained_Border(Polyhedron & p, double stopCondition);
	static void surfaceSimplification_Edge_Collapse(Polyhedron & p, double stopCondition);

	static void polyhedronMerge(Polyhedron & p1, Polyhedron & p2, Polyhedron & meged_polyhedron);
	
	static void polyhedronSplit(Polyhedron & poly, Point_3 &v1, Point_3 &v2, Point_3 &v3, std::vector<Polyhedron> & splited_polyhedron_vector);
	static void polyhedronSplit(Polyhedron & poly, Plane_3 & plane, std::vector<Polyhedron> & splited_polyhedron_vector);
	static void make_pca_plane_3(Polyhedron & poly, Plane_3 & pca_plane);
};