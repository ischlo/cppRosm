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


/////////// 


double numeric_vector_mean(Rcpp::NumericVector vec){
  if (vec.size()==0) return 0;
  double total = 0;
  for(Rcpp::NumericVector::iterator i = vec.begin(); i != vec.end(); ++i) {
    total += *i;
  }
  return total/vec.size();
}


class RoadSegmentExtractor : public osmium::handler::Handler {
public:
  
  RoadSegmentExtractor(const std::string& out){
    
    segment_csv = std::ofstream(out + "road_segments.csv");
    node_csv = std::ofstream(out + "nodes.csv");
    
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
      
      double len;
      
      for(long unsigned int i(0); i < nodes.size()-1; ++i){
        // Extract from/to node ids and calculate segment length
        from_node_id = nodes[i];
        to_node_id = nodes[i+1];
        
        p1=geom::Coordinates(from_node_id.location());
        p2=geom::Coordinates(to_node_id.location());
        
        len = osmium::geom::haversine::distance(p1,p2);
        
        // Write segment data to CSV
        segment_csv << from_node_id.ref() << "," << to_node_id.ref() << "," << len << "," << highway << "\n"; // << length  figure out the optimal way to calculate the length and then ad
        
        // from.push_back(std::to_string(
        //     from_node_id.ref()
        //                  ));
        // to.push_back(std::to_string(
        //     to_node_id.ref()
        //                ));
        // length.push_back(
        //   len
        //   );
        // highway_key.push_back(
        //   highway
        //   );
        // 
      }
      
      // Write nodes to CSV
      for (const auto& node : nodes) {
        
        node_csv << node.ref() << "," << node.location().lon() << "," << node.location().lat() << "\n";
        
        // id.push_back(std::to_string(
        //     node.ref()
        //                ));
        // lon.push_back(node.location().lon());
        // lat.push_back(node.location().lat());
        
      }
    }
  };
  
  
  // Rcpp::DataFrame export_segments(){
  // 
  //   return Rcpp::DataFrame::create(
  //     Rcpp::Named("from")=Rcpp::clone(from)
  //   ,Rcpp::Named("to")=Rcpp::clone(to)
  //   ,Rcpp::Named("length")=Rcpp::clone(length)
  //   ,Rcpp::Named("highway")=Rcpp::clone(highway_key)
  //   );
  // 
  // }
  // 
  // Rcpp::DataFrame export_nodes(){
  //  
  //   return Rcpp::DataFrame::create(
  //     Rcpp::_["id"]=Rcpp::clone(id)
  //   ,Rcpp::_["lon"]=Rcpp::clone(lon)
  //   ,Rcpp::_["lat"]=Rcpp::clone(lat)
  //   );
  // }

  // Rcpp::List export_network(){
  //   
  //   return Rcpp::List::create(
  //     Rcpp::_["nodes"]=Rcpp::DataFrame::create(
  //       Rcpp::_["id"]=Rcpp::clone(id)
  //       ,Rcpp::_["lon"]=Rcpp::clone(lon)
  //       ,Rcpp::_["lat"]=Rcpp::clone(lat)
  //   )
  //   ,Rcpp::_["segments"]=Rcpp::DataFrame::create(
  //     Rcpp::_["from"]=Rcpp::clone(from)
  //     ,Rcpp::_["to"]=Rcpp::clone(to)
  //     ,Rcpp::_["length"]=Rcpp::clone(length)
  //     ,Rcpp::_["highway"]=Rcpp::clone(highway_key)
  //   ));
  // }
  
private:
  
  // replace this with provate Dataframes that store all the data and just return it into R ?
  // optionnaly exporting them from the R function. 
  
  // Rcpp::CharacterVector from, to, id, highway_key;
  // Rcpp::NumericVector lat,lon,length;
  
  std::ofstream segment_csv;
  // {"road_segments.csv"};
  std::ofstream node_csv;
  // {"nodes.csv"};
  
};


class general_extractor : public osmium::handler::Handler{
public:
  
  general_extractor(Rcpp::CharacterVector main_sel){
    first_level_keys = main_sel;
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
  
  void way(const osmium::Way& way){
    
    // only polygons are exported here
    // , so we check for a main key, then 
    // check that the way is closed -> last node is same as first
    // and if so, we add the node list with coordinates in the attrs values as a data frame (list ? ). 
    // in the package, there can be a function like : reconstruct_geom that will take all these ways and make them into polygons. 
    // but we will also find the centroid of the way and put it into the x,y variables.
    
    main_key.clear();
    main_val.clear();
    id_indiv.clear();
    
    if(!way.is_closed()) return;
    
    set_main_key(way.tags());
    
    if(main_key.empty()){
      return;  
    } else {
      // main_val = way.tags().get_value_by_key(main_key);
      id_indiv=std::to_string(way.id());
      attr = get_attr_list(way.tags());
      
      Rcpp::DataFrame nodes = accumulate_nodes(way.nodes());
      
      attr["nodes"] = nodes;
      
      // finnaly get the coordinates and put it all into the Rcpp::vectors. 
      id.push_back(id_indiv);
      
      main_keys.push_back(main_key);
      
      main_values.push_back(main_val);
      
      attrs.push_back(attr);
      
      x.push_back(numeric_vector_mean(nodes["x"]));
    
      y.push_back(numeric_vector_mean(nodes["y"]));
      
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
  
  Rcpp::CharacterVector first_level_keys;
  
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

    for (const auto& key: first_level_keys){
      if(tags.has_key(key)){

        main_key=key;
        main_val= tags.get_value_by_key(key);
      }
    }

    // find the right return type to keep it simple.
    return;
  }
  
  Rcpp::DataFrame accumulate_nodes(const WayNodeList& way_nodes){
    
    Rcpp::CharacterVector node_id;
    Rcpp::NumericVector node_x,node_y;
    
    for (const osmium::NodeRef& nr : way_nodes) {
      node_id.push_back(std::to_string(nr.ref()));
      node_x.push_back(nr.lon());
      node_y.push_back(nr.lat());
    }
    
    return Rcpp::DataFrame::create(Rcpp::_["node_id"] = node_id
                                     ,Rcpp::_["x"] = node_x
                                     ,Rcpp::_["y"] = node_y
                                     );
    
  }

};


//@export
// [[Rcpp::export]]
int cpp_extract_graph(const std::string& file, const std::string& out = ""){
  
  // Rcpp::List 
  
  // , bool key_vals=true ; when more parameters will be implemented.
  
  // The index to hold node locations.
  index_type index;
  
  // The location handler will add the node locations to the index and then
  // to the ways
  location_handler_type location_handler{index};
  
  const std::string input_file(file);
  
  Reader input_file_reader(input_file);
  
  RoadSegmentExtractor handler(out);
  
  // Apply the handler to the input file
  apply(input_file_reader, location_handler, handler);
  
  return 1;
}

// @export
// [[Rcpp::export]]
Rcpp::List cpp_extract_data(const std::string& file,Rcpp::CharacterVector main_sel){
  
  // The index to hold node locations.
  index_type index;
  
  // The location handler will add the node locations to the index and then
  // to the ways
  location_handler_type location_handler{index};
  
  const std::string input_file(file);
  
  Reader input_file_reader(input_file);
  
  general_extractor general_handler(main_sel);
  
  // Apply the handler to the input file
  apply(input_file_reader,location_handler, general_handler);
  
  return(general_handler.export_data());
}
