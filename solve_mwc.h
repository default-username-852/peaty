#include "params.h"
#include "sparse_graph.h"

struct Result
{
    VtxList vertex_cover;
    long search_node_count;
    Result(const SparseGraph & g);
    ~Result();
};

auto mwc(SparseGraph* g, const Params* params) -> Result;
bool check_vertex_cover(const SparseGraph & g, const vector<int> & vc);
