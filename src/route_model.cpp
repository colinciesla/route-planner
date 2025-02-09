#include "route_model.h"

#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml)
  : Model(xml) {
  // Create RouteModel nodes.
  int counter = 0;
  for (Model::Node node : this->Nodes()) {
    m_Nodes.emplace_back(counter, this, node);
    counter++;
  }
  CreateNodeToRoadHashmap();
}

void RouteModel::CreateNodeToRoadHashmap() {
  for (const Model::Road &road : Roads()) {
    if (road.type != Model::Road::Type::Footway) {
      for (int node_idx : Ways()[road.way].nodes) {
        if (node_to_road.find(node_idx) == node_to_road.end()) {
          node_to_road[node_idx] = std::vector<const Model::Road*>();
        }
        node_to_road[node_idx].push_back(&road);
      }
    }
  }
}

RouteModel::Node* RouteModel::Node::FindNeighbor(const std::vector<int> &node_indices) const {
  Node* closest_node = nullptr;

  for (const int node_index : node_indices) {
    if (Node node = parent_model->SNodes()[node_index];
        this->distance(node) != 0 && !node.visited) {
      if (closest_node == nullptr || this->distance(node) < this->distance(*closest_node)) {
        closest_node = &parent_model->SNodes()[node_index];
      }
    }
  }
  return closest_node;
}

void RouteModel::Node::FindNeighbors() {
  for (const auto &road : parent_model->node_to_road[this->index]) {
    if (Node* new_neighbor = this->FindNeighbor(parent_model->Ways()[road->way].nodes)) {
      this->neighbors.emplace_back(new_neighbor);
    }
  }
}

RouteModel::Node &RouteModel::FindClosestNode(const float x, const float y) {
  Node input;
  input.x = x;
  input.y = y;

  float min_dist = std::numeric_limits<float>::max();
  int closest_idx = 0;

  for (const auto &[roadWay, roadType] : Roads()) {
    if (roadType != Road::Type::Footway) {
      for (const int node_idx : Ways()[roadWay].nodes) {
        if (const float dist = input.distance(SNodes()[node_idx]); dist < min_dist) {
          closest_idx = node_idx;
          min_dist = dist;
        }
      }
    }
  }

  return SNodes()[closest_idx];
}
