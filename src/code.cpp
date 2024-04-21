#include <Rcpp.h>
#include <osmium/io/xml_input.hpp>
#include <osmium/io/pbf_input.hpp>
#include <osmium/visitor.hpp>
#include <osmium/handler.hpp>
#include <fstream>
#include <iostream>
#include <string>
// For the osmium::geom::haversine::distance() function
#include <osmium/geom/haversine.hpp>
// For the location index. There are different types of indexes available.
// This will work for all input files keeping the index in memory.
#include <osmium/index/map/flex_mem.hpp>
// For the NodeLocationForWays handler
#include <osmium/handler/node_locations_for_ways.hpp>

// #include <osmium/io/any_input.hpp>

// The type of index used. This must match the include file above
using index_type = osmium::index::map::FlexMem<osmium::unsigned_object_id_type, osmium::Location>;
// The location handler always depends on the index type
using location_handler_type = osmium::handler::NodeLocationsForWays<index_type>;

using namespace osmium;
using namespace osmium::io;
using namespace osmium::osm_entity_bits;
using namespace Rcpp;


class RoadSegmentExtractor : public osmium::handler::Handler {
public:
  
  RoadSegmentExtractor(){
    segment_csv << "from," << "to," << "length," << "highway" << "\n";
    node_csv << "id," << "lon," << "lat"<<"\n"; 
  };
  
  void way(const osmium::Way& way) {
    // Check if the way represents a road segment
    if (way.tags().has_key("highway")) {
      // Extract nodes of the road segment
      const auto& nodes = way.nodes();
      if (nodes.size() < 2) return; // Skip if there are fewer than 2 nodes
      
      // std::cout << way.tags()["highway"] << "\n";
      
      std::string highway = way.tags().get_value_by_key("highway");
      
      osmium::NodeRef from_node_id,to_node_id;
      osmium::geom::Coordinates p1,p2;
      
      for(long unsigned int i(0); i<nodes.size()-1; ++i){
        // Extract from/to node ids and calculate segment length
        from_node_id = nodes[i];
        to_node_id = nodes[i+1];
        
        p1=geom::Coordinates(from_node_id.location());
        p2=geom::Coordinates(to_node_id.location());
        // const auto length = calculate_segment_length(nodes);
        // Write segment data to CSV
        segment_csv << from_node_id.ref() << "," << to_node_id.ref() << "," << osmium::geom::haversine::distance(p1,p2) << "," << highway << "\n"; // << length  figure out the optimal way to calculate the length and then add
        
        // node_set.clear();
        
      }
      
      // Write nodes to CSV
      for (const auto& node : nodes) {
        node_csv << node.ref() << "," << node.location().lon() << "," << node.location().lat() << "\n";
      }
    }
  };
  
  // void init_stream(){
  //     segment_csv << "from," << "to," << "length," << "highway" << "\n";
  //     node_csv << "id," << "lon," << "lat" << "\n"; 
  // }
  
private:
  // double calculate_segment_length(const WayNodeList& nodes) {
  //     double length = 0.0;
  //     for (std::size_t i = 1; i < nodes.size(); ++i) {
  //         length += nodes[i - 1].location().distance(nodes[i].location());
  //     }
  //     return length;
  // }
  std::ofstream segment_csv{"road_segments.csv"};
  
  std::ofstream node_csv{"nodes.csv"};
  
  
};


//@export
// [[Rcpp::export]]
int cpp_extract_graph(const std::string& file){
  
  // , bool key_vals=true ; when more parameters will be implemented.
  
  // The index to hold node locations.
  index_type index;
  
  // The location handler will add the node locations to the index and then
  // to the ways
  location_handler_type location_handler{index};
  
  const std::string input_file(file);
  
  Reader input_file_reader(input_file);
  
  RoadSegmentExtractor handler;
  
  // Apply the handler to the input file
  apply(input_file_reader, location_handler, handler);
  
  return 1;
}

// 
// int main(int argc, char* argv[]) {
//   if (argc != 2) {
//     std::cerr << "Usage: " << argv[0] << " <input-file>" << std::endl;
//     exit(1);
//   }
//   
//   // The index to hold node locations.
//   index_type index;
//   
//   // The location handler will add the node locations to the index and then
//   // to the ways
//   location_handler_type location_handler{index};
//   
//   const std::string input_file(argv[1]);
//   
//   Reader input_file_reader(input_file);
//   
//   RoadSegmentExtractor handler;
//   
//   // Apply the handler to the input file
//   apply(input_file_reader, location_handler, handler);
//   
//   return 0;
// }