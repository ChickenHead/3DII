#ifndef CGLAB_FACET_H
#define CGLAB_FACET_H 1

#include <CGAL/HalfedgeDS_face_base.h>
#include <CGAL/Polyhedron_items_3.h>

template <class Refs, class T, class Norm>
class CGLab_facet : public CGAL::HalfedgeDS_face_base<Refs, T> {
    Norm  norm;
    bool visited;
    bool groupped;
    int fid;
public:
    // no constructors to repeat, since only default constructor mandatory
    CGLab_facet() { visited = false; groupped = false; fid = -1; }
    typedef Norm Normal_3;
    Normal_3&       normal()       { return norm; }
    const Normal_3& normal() const { return norm; }
    
    bool is_visited() { return visited; }
    void setVisited(bool b) { visited = b; }
    
    bool is_groupped() { return groupped; }
    void setGroupped(bool g) { groupped = g; }
    
    int Fid() { return fid; }
    void setFid(int id) { fid = id; }
};

struct CGLab_items : public CGAL::Polyhedron_items_3 {
    template <class Refs, class Traits>
    struct Face_wrapper {
        typedef typename Traits::Vector_3 Normal;
        typedef CGLab_facet<Refs, CGAL::Tag_true, Normal> Face;
    };
};

struct Facet_normal {
    template <class Facet>
    void operator()( Facet& f) {
        typename Facet::Halfedge_handle h = f.halfedge();
        typename Facet::Normal_3 normal = CGAL::cross_product(
                                                              h->next()->vertex()->point() - h->vertex()->point(),
                                                              h->next()->next()->vertex()->point() - h->next()->vertex()->point());
        f.normal() = normal / std::sqrt( normal * normal);
    }
};

#endif // CGLAB_FACET_H
