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
        
        // Write segment data to CSV
        segment_csv << from_node_id.ref() << "," << to_node_id.ref() << "," << osmium::geom::haversine::distance(p1,p2) << "," << highway << "\n"; // << length  figure out the optimal way to calculate the length and then ad
        
      }
      
      // Write nodes to CSV
      for (const auto& node : nodes) {
        node_csv << node.ref() << "," << node.location().lon() << "," << node.location().lat() << "\n";
      }
    }
  };
  
private:
  
  // replace this with provate Dataframes that store all the data and just return it into R ?
  // optionnaly exporting them from the R function. 
  
  std::ofstream segment_csv{"road_segments.csv"};
  
  std::ofstream node_csv{"nodes.csv"};
  
};


class general_extractor : public osmium::handler::Handler{
public:
  
  general_extractor(){
    
  }
  
  void node(const osmium::Node& node){
    
    main_key.clear();
    main_val.clear();
    id_indiv.clear();
    
    set_main_key(node.tags());
    
    if(main_key.empty()){
      return;  
    } else {
      // main_val = node.tags().get_value_by_key(main_key);
      id_indiv=std::to_string(node.id());
      attr = get_attr_list(node.tags());
      
      // finnaly get the coordinates and put it all into the Rcpp::vectors. 
      id.push_back(id_indiv);
      
      main_keys.push_back(main_key);
      
      main_values.push_back(main_val);
  
      attrs.push_back(attr);
      
      x.push_back(node.location().lon());
      
      y.push_back(node.location().lat());
      
    }
    
  }
  
  Rcpp::List export_data(){
    return Rcpp::List::create(Rcpp::Named("id")=Rcpp::clone(id)
                                     ,Rcpp::Named("key")=Rcpp::clone(main_keys)
                                     ,Rcpp::Named("value")=Rcpp::clone(main_values)
                                     ,Rcpp::Named("lon")=Rcpp::clone(x)
                                     ,Rcpp::Named("lat")=Rcpp::clone(y)
                                     ,Rcpp::Named("attrs")=Rcpp::clone(attrs)
    );
  }
    
private:
  
  Rcpp::CharacterVector first_level_keys = {"aerialway"
    ,"aeroway"
    ,"barrier"
    ,"boundary"
    ,"building"
    ,"craft"
    ,"emergency"
    ,"geological"
    ,"healthcare"
    // ,"highway" this is treated by a separate function
    ,"historic"
    ,"landuse"
    ,"leisure"
    ,"man_made"
    ,"military"
    ,"natural"
    ,"office"
    ,"place"
    ,"power"
    ,"public_transport"
    ,"railway"
    ,"shop"
    ,"telecom"
    ,"tourism"
    ,"waterway"
    };
  
  // individual values to iterate.
  
  std::string main_key,main_val,id_indiv;
  
  Rcpp::List attr;
  
  // accumulated vector values 
  
  Rcpp::CharacterVector id, main_keys, main_values;
  
  Rcpp::NumericVector x,y;
  
  Rcpp::List attrs;
  
  // private methods
  
  Rcpp::List get_attr_list(const TagList& tagl){
    // iterate other all the tags that don't correspond tot he main key and 
    // add them to the attr list as 'key'='value' named list element.
    Rcpp::List res;
    
    for(const auto& tag: tagl){
      if(tag.key()!=main_key){
        // here append to the res list all the other tags, with key to name and value to value. 
        res[tag.key()]=tag.value();
      }
    }
    return res;
  };
  
  void set_main_key(const osmium::TagList& tags){
    // std::string main_key;
    // std::string main_val;
    // char *res='none';

    for (const auto& key: first_level_keys){
      if(tags.has_key(key)){

        main_key=key;
        main_val= tags.get_value_by_key(key);
      }
    }

    // find the right return type to keep it simple.
    return;
  }

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

// @export
// [[Rcpp::export]]
Rcpp::List cpp_extract_data(const std::string& file){
  
  // The index to hold node locations.
  // index_type index;
  
  // The location handler will add the node locations to the index and then
  // to the ways
  // location_handler_type location_handler{index};
  
  const std::string input_file(file);
  
  Reader input_file_reader(input_file);
  
  general_extractor general_handler;
  
  // Apply the handler to the input file
  apply(input_file_reader, general_handler);
  
  return(general_handler.export_data());
}
