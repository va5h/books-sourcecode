define([
  "dojo/_base/declare",
  "dojo/_base/array",
  "dojo/_base/lang",
  "dojo/dom",
  "dojo/dom-style",
  "dojo/dom-construct",
  "dojo/json",
  "dojo/on",

  "dijit/_WidgetBase",
  "dijit/_TemplatedMixin",
  "dijit/_OnDijitClickMixin",

  "dojo/text!./templates/Census.html",
  "dojo/text!./templates/StateCensus.html",
  "dojo/text!./templates/CountyCensus.html",
  "dojo/text!./templates/BlockGroupCensus.html",
  "dojo/text!./templates/BlockCensus.html",

  "dojox/charting/Chart",
  "dojox/charting/plot2d/Pie",
  "dojox/charting/plot2d/Bars",
  "dojox/charting/action2d/Highlight",
  "dojox/charting/action2d/Tooltip",
  "dojox/charting/themes/PrimaryColors",
  "dojox/charting/SimpleTheme",

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
  dom, domStyle, domConstruct, JSON, dojoOn,
  _WidgetBase, _TemplatedMixin, _OnDijitClickMixin,
  dijitTemplate,
  StateTemplate, CountyTemplate, BlockGroupTemplate, BlockTemplate,
  Chart, Pie, Bars, Highlight, Tooltip, PrimaryColors, SimpleTheme,
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

        this.map.infoWindow.resize(300, 300);

        dojoOn(this.map.infoWindow, "selection-change", lang.hitch(this, this._onInfoWindowSelect));
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
            title += " ${STATE_FIPS}";
            content = BlockTemplate;
            break;
          case "Census Block Group":
          	title += " ${BLKGRP}";
            content = BlockGroupTemplate;
            break;
          case "Detailed Counties":
            title = "${NAME}";
            content = CountyTemplate;
            break;
          case "states":
          	title = "${STATE_NAME}";
            content = StateTemplate;
            break;
          default:
            content = "${*}";
        }

        feature.infoTemplate = new InfoTemplate(title, content);

        return feature;
    },

    _onInfoWindowSelect: function () {
      var graphic = this.map.infoWindow.getSelectedFeature(),
    	  ethnicData, genderData, ageData;
    	if (graphic && graphic.attributes) {
    		// load and render the ethnic data
    		ethnicData = this.ethnicData(graphic.attributes);
    		this.ethnicGraph(ethnicData);
    		// load and render the gender data
    		genderData = this.genderData(graphic.attributes);
    		this.genderGraph(genderData);
    		// load and render the age data
    		ageData = this.ageData(graphic.attributes);
    		this.ageGraph(ageData);
    	}
    },

    formatAttributesForGraph: function (attributes, fieldLabels) {
      var data = [],
        field;
      for (field in fieldLabels) {
        data.push({
          tooltip: fieldLabels[field],
          y: +attributes[field]
        })
      }
      return data;
    },

    ethnicData: function (attributes) {
      var fieldLabels = {
        "WHITE": "Caucasian",
        "BLACK": "African-American",
        "AMERI_ES": "Native American /<br> Alaskan Native",
        "ASIAN": "Asian",
        "HAWN_PI": "Hawaiian /<br> Pacific Islander",
        "HISPANIC": "Hispanic",
        "OTHER": "Other",
        "MULT_RACE": "Multiracial"
      };

      return this.formatAttributesForGraph(attributes, fieldLabels);
    },

    ethnicGraph: function (data) {
    	var ethnicChart = new Chart("ethnicgraph");
	    ethnicChart.setTheme(PrimaryColors)
	     .addPlot("default", {
	        type: Pie,
	        font: "normal normal 11pt Tahoma",
	        fontColor: "black",
	        radius: 90
	    }).addSeries("Series A", data);
	    var anim_a = new Tooltip(ethnicChart, "default");
	    var anim_b = new Highlight(ethnicChart, "default");
	    ethnicChart.render();
    },

    genderData: function (attributes) {
      var fieldLabels = {
        "MALES": "Males",
        "FEMALES": "Females"
      };

    	return this.formatAttributesForGraph(attributes, fieldLabels);
    },

    genderGraph: function (data) {
    	var genderTheme = new SimpleTheme({
    		colors: ["#8888ff", "#ff8888"]
    	});
    	var genderChart = new Chart("gendergraph");
	    genderChart.setTheme(genderTheme)
	     .addPlot("default", {
	        type: Pie,
	        font: "normal normal 11pt Tahoma",
	        fontColor: "black",
	        radius: 90
	    }).addSeries("Series A", data);
	    var anim_a = new Tooltip(genderChart, "default");
	    var anim_b = new Highlight(genderChart, "default");
	    genderChart.render();
    },

    ageData: function (attributes) {
      var fieldLabels = {
        "AGE_UNDER5": "&lt; 5",
        "AGE_5_17": "5-17",
        "AGE_18_21": "18-21",
        "AGE_22_29": "22-29",
        "AGE_30_39": "30-39",
        "AGE_40_49": "40-49",
        "AGE_50_64": "50-64",
        "AGE_65_UP": "&gt; 64"
      };

      return this.formatAttributesForGraph(attributes, fieldLabels);
    },

    ageGraph: function (data) {
    	var ageChart = new Chart("agegraph");
	    ageChart.setTheme(PrimaryColors)
	     .addPlot("default", {
	        type: Bars,
	        font: "normal normal 11pt Tahoma",
	        fontColor: "black",
	        gap: 2,
	        maxBarSize: 220
	    }).addSeries("Series A", data);
	    var anim_a = new Tooltip(ageChart, "default");
	    var anim_b = new Highlight(ageChart, "default");
	    ageChart.render();
    }
  });
});
