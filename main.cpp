#include <argp.h>
#include <string.h>

#include "solve_mwc.h"

static char doc[] = "Find a maximum clique in a graph in DIMACS format";
static char args_doc[] = "";
static struct argp_option options[] = {
    {"quiet", 'q', 0, 0, "Quiet output"},
    {"unweighted-sort", 'u', 0, 0, "Unweighted ordering (only applies to certain algorithms)"},
    {"colouring-variant", 'c', "VARIANT", 0, "For algorithms 0 and 5, which type of colouring?"},
    {"algorithm", 'a', "NUMBER", 0, "Algorithm number"},
    {"max-sat-level", 'm', "LEVEL", 0, "Level of MAXSAT reasoning; default=2"},
    {"num-threads", 't', "NUMBER", 0, "Number of threads (for parallel algorithms only)"},
    {"file-format", 'f', "FORMAT", 0, "File format (DIMACS, MTX or EDGES)"},
    { 0 }
};

enum class FileFormat
{
    Dimacs,
    Pace
};

static struct {
    bool quiet;
    bool unweighted_sort;
    int colouring_variant = 3;
    int algorithm_num;
    int max_sat_level = -1;
    int num_threads = 1;
    FileFormat file_format = FileFormat::Pace;
} arguments;

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
    switch (key) {
        case 'q':
            arguments.quiet = true;
            break;
        case 'u':
            arguments.unweighted_sort = true;
            break;
        case 'c':
            arguments.colouring_variant = atoi(arg);
            break;
        case 'a':
            arguments.algorithm_num = atoi(arg);
            break;
        case 'm':
            arguments.max_sat_level = atoi(arg);
            break;
        case 't':
            arguments.num_threads = atoi(arg);
            break;
        case 'f':
            if (!strcmp(arg, "PACE") || !strcmp(arg, "pace"))
                arguments.file_format = FileFormat::Pace;
            else if (!strcmp(arg, "DIMACS") || !strcmp(arg, "dimacs"))
                arguments.file_format = FileFormat::Dimacs;
            break;
        case ARGP_KEY_ARG:
//            argp_usage(state);
            break;
        case ARGP_KEY_END:
            break;
        default: return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char** argv) {
    argp_parse(&argp, argc, argv, 0, 0, 0);

    if (arguments.algorithm_num != 5)
        arguments.num_threads = 1;

    SparseGraph g =
            arguments.file_format==FileFormat::Pace ? readSparseGraphPaceFormat() :
                                                      readSparseGraph();

    Params params {arguments.colouring_variant, arguments.max_sat_level, arguments.algorithm_num,
            arguments.num_threads, arguments.quiet, arguments.unweighted_sort};

    Result result = mwc(&g, &params);

    // sort vertices in clique by index
    std::sort(result.vertex_cover.vv.begin(), result.vertex_cover.vv.end());

    std::cout << "s vc " << g.n << " " << result.vertex_cover.vv.size() << std::endl;
    for (int v : result.vertex_cover.vv)
        std::cout << (v+1) << std::endl;

//    printf("Stats: status program algorithm_number max_sat_level num_threads size weight nodes\n");
//    std::cout <<
//            (aborted ? "TIMEOUT" : "COMPLETED") << " " <<
//            argv[0] << " " <<
//            arguments.algorithm_num << " " <<
//            arguments.max_sat_level << " " <<
//            arguments.num_threads << " " <<
//            result.vertex_cover.vv.size() << " " <<
//            result.vertex_cover.total_wt <<  " " <<
//            result.search_node_count << std::endl;

    if (!check_vertex_cover(g, result.vertex_cover.vv))
        fail("*** Error: invalid solution\n");
}
