require([
  "dojo/parser",
  "dojo/_base/lang",
  "dojo/dom",
  "dojo/on",
  "esri/map",
  "esri/arcgis/utils",
  "y2k/bootstrapmap",
  "y2k/Census",
  "bootstrap/Dropdown",
  "bootstrap/Modal",
  "dojo/domReady!"
], function(
  parser, lang, dom, dojoOn, Map, arcgisUtils, BootstrapMap, Census
) {
  parser.parse();
  
  /*
  var deferred = arcgisUtils.createMap("450d4fb709294359ac8d03a3069e34d3", "map", {});
   */
  
  var deferred = BootstrapMap.createWebMap("450d4fb709294359ac8d03a3069e34d3", "map", {});
  
  deferred.then(function (response) {
    var map = response.map;
    
    var census = new Census({
      map: map,
      mapService: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/"
    }, "census-widget");
  
    dojoOn(dom.byId("census-btn"), "click", lang.hitch(census, census.show));
  });
  
  
  
});