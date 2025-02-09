#ifndef ROUTE_MODEL_H
#define ROUTE_MODEL_H

#include <cmath>
#include <limits>
#include <unordered_map>

#include "model.h"

class RouteModel : public Model {
  public:
    class Node : public Model::Node {
      public:
        Node* parent = nullptr;
        float h_value = std::numeric_limits<float>::max();
        float g_value = 0.0;
        bool visited = false;
        std::vector<Node*> neighbors;

        void FindNeighbors();
        [[nodiscard]] float distance(const Node &other) const {
          return std::sqrt(
            static_cast<float>(std::pow((x - other.x), 2) + std::pow((y - other.y), 2)));
        }

        Node() = default;
        Node(const int idx, RouteModel* search_model, const Model::Node node)
          : Model::Node(node),
            index(idx),
            parent_model(search_model) {
        }

      private:
        int index {};
        [[nodiscard]] Node* FindNeighbor(const std::vector<int> &node_indices) const;
        RouteModel* parent_model = nullptr;
    };

    explicit RouteModel(const std::vector<std::byte> &xml);
    Node &FindClosestNode(float x, float y);
    auto &SNodes() {
      return m_Nodes;
    }
    std::vector<Node> path;

  private:
    void CreateNodeToRoadHashmap();
    std::unordered_map<int, std::vector<const Road*>> node_to_road;
    std::vector<Node> m_Nodes;
};

#endif
