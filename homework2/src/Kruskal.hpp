#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include "Graph.hpp"
#include <vector>
#include <tuple>

class Kruskal
{
public:
    // 回傳 MST (邊的集合, 總權重)
    static std::pair<std::vector<std::tuple<int, int, int>>, int> findMST(const Graph& graph);
};

#endif // KRUSKAL_HPP
