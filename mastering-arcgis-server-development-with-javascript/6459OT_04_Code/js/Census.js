define([
  "dojo/_base/declare",
  "dojo/_base/array",
  "dojo/_base/lang",
  "dojo/dom",
  "dojo/dom-style",
  "dojo/dom-construct",
  "dojo/json",
  
  "dijit/_WidgetBase",
  "dijit/_TemplatedMixin",
  "dijit/_OnDijitClickMixin",
  
  "dojo/text!./templates/Census.html",
  "dojo/text!./templates/StateCensus.html",
  "dojo/text!./templates/CountyCensus.html",
  "dojo/text!./templates/BlockGroupCensus.html",
  "dojo/text!./templates/BlockCensus.html",
  
  "esri/tasks/IdentifyParameters",
  "esri/tasks/IdentifyTask",
  "esri/InfoTemplate",
  "esri/symbols/SimpleMarkerSymbol",
  "esri/symbols/SimpleFillSymbol",
  "esri/request",
  "esri/tasks/query",
  "esri/tasks/QueryTask",
  "esri/config"
], function (
  declare, arrayUtils, lang,
  dom, domStyle, domConstruct, JSON,
  _WidgetBase, _TemplatedMixin, _OnDijitClickMixin,
  dijitTemplate,
  StateTemplate, CountyTemplate, BlockGroupTemplate, BlockTemplate,
  IdentifyParameters, IdentifyTask,
  InfoTemplate, MarkerSymbol, FillSymbol,
  esriRequest, Query, QueryTask, esriConfig
) {
  return declare([_WidgetBase, _TemplatedMixin, _OnDijitClickMixin], {
    
    baseClass: "y2k-census",
    declaredClass: "y2k.Census",
    templateString: dijitTemplate,
    constructor: function (options, srcRefNode) {
      
      if (typeof srcRefNode === "string") {
      	srcRefNode = dom.byId(srcRefNode)
      }
      
      this.identifyTask = new IdentifyTask(options.mapService);
      this.map = options.map || null;
      this.domNode = srcRefNode;
      
      
      
      if (this.map.loaded) {
	    this._onMapLoad();
	  } else {
	    this.map.on("load", lang.hitch(this, this._onMapLoad));
	  }
    },
    
    /*
    // new
    postCreate: function () {
    	var url = this.identifyTask.url;
    	
    	this.inherited(arguments);
    	
    	esriRequest({
    		url: url,
    		handleAs: "json", 
    		content: {f: "json"}
    	}).then(lang.hitch(this, function (response) {
    		// check over each layer.
    		arrayUtils.forEach(response.layers, lang.hitch(this, function (layer) {
    			// make sure it's not a group layer
    			if (layer.subLayerIds == null) {
    				// add an option with the layer name for the content and url for the value.
    				domConstruct.create("option", {
    					value: url + layer.id,
    					innerHTML: layer.name
    				}, this.mapLayerSelect);
    			}
    		}));
    	}));
    },
    
    //new
    _getFieldsFromLayer: function (url) {
    	esriRequest({
    		url: url,
    		handleAs: "json", 
    		content: {f: "json"}
    	}).then(lang.hitch(this, function (result) {
    		
    		var fields = result.fields;
    		
    		fields = arrayUtils.forEach(fields, lang.hitch(this, function (field) {
    			if (field.type === "esriFieldTypeOID" && field.type === "esriFieldTypeGeometry") {
    				return;
    			}
    			
    			domConstruct.create("option", {
    				value: JSON.stringify(field),
    				innerHTML: field.alias
    			}, this.layerFieldSelect);
    		}));
    	}));
    },
    */
   
    //new
    _onMapLoad: function () {
    	// change the infoWindow symbol
        this.map.infoWindow.markerSymbol = new MarkerSymbol({
			style: "esriSMSDiamond",
			color: [255,200,30],
			size: 12,
			outline: {
				width: 1,
				color: [111,98,34]
			}
		});
		
		this.map.infoWindow.fillSymbol = new FillSymbol({
			style: "esriSFSSolid",
			color: [255,250,169,128],
			outline: {
				width: 2,
				color: [111,98,34]
			}
		});
        
    },
    
    show: function () {
      domStyle.set(this.domNode, "display", "block");
      this._mapClickHandler = this.map.on("click", lang.hitch(this, this._onMapClick));
    },
    
    hide: function () {
      domStyle.set(this.domNode, "display", "none");
      if (this._mapClickHandler && typeof this._mapClickHandler.remove === "function") {
        this._mapClickHandler.remove();
      }
    },
    
    _onMapClick: function (event) {
      var params = new IdentifyParameters(),
        defResults;
            
      params.geometry = event.mapPoint;
      params.layerOption = IdentifyParameters.LAYER_OPTION_ALL;
      params.mapExtent = this.map.extent;
      params.returnGeometry = true;
      params.width = this.map.width;
      params.height= this.map.height;
      params.spatialReference = this.map.spatialReference;
      params.tolerance = 3;
      
      this.map.graphics.clear();
      defResults = this.identifyTask.execute(params).addCallback(lang.hitch(this, this._onIdentifyComplete));
      this.map.infoWindow.setFeatures([defResults]);
        this.map.infoWindow.show(event.mapPoint);
    },
    
    // changed
    _onIdentifyComplete: function (results) {
      
      return arrayUtils.map(results, lang.hitch(this, function (result) {
        return this._processFeature(result.feature, result.layerName);
      }));
    }, 
    
    // new
    _processFeature: function (feature, title) {
    	var content;
    	
    	switch(title) {
          case "Census Block Points":
            content = BlockTemplate;
            break;
          case "Census Block Group":
            content = BlockGroupTemplate;
            break;
          case "Detailed Counties":
            content = CountyTemplate;
            break;
          case "states":
            content = StateTemplate;
            break;
          default:
            content = "${*}";
        }
        
        feature.infoTemplate = new InfoTemplate(title, content);
        
        return feature;
    },
    
    /*
    _layerChange: function (event) {
    	var url = this.mapLayerSelect.value;
    	
    	this._getFieldsFromLayer(url);
    },
    
    _fieldChange: function (event) {
    	
    },
    
    _search: function (event) {
    	
    }
    */
  });
});