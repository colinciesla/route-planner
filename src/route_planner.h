#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include <vector>

#include "route_model.h"

class RoutePlanner {
  public:
    RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y);
    // Add public variables or methods declarations here.
    [[nodiscard]] float GetDistance() const {
      return distance;
    }
    void AStarSearch();

    // The following methods have been made public so we can test them individually.
    void AddNeighbors(RouteModel::Node* current_node) const;
    float CalculateHValue(RouteModel::Node const* node) const;
    void AddNeighbors(RouteModel::Node* current_node);
    std::vector<RouteModel::Node> ConstructFinalPath(const RouteModel::Node*);
    RouteModel::Node* NextNode();

  private:
    // Add private variables or methods declarations here.
    std::vector<RouteModel::Node*> open_list;
    RouteModel::Node* start_node {};
    RouteModel::Node* end_node {};

    float distance = 0.0f;
    RouteModel &m_Model;
};

#endif
