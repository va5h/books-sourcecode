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

  "esri/tasks/IdentifyParameters",
  "esri/tasks/IdentifyTask",
  "esri/InfoTemplate",
  "esri/symbols/SimpleMarkerSymbol",
  "esri/symbols/SimpleFillSymbol",
  "esri/request",
  "esri/tasks/query",
  "esri/tasks/QueryTask",
  "esri/config",
  "d3/d3"
], function (
  declare, arrayUtils, lang,
  dom, domStyle, domConstruct, JSON, dojoOn,
  _WidgetBase, _TemplatedMixin, _OnDijitClickMixin,
  dijitTemplate,
  StateTemplate, CountyTemplate, BlockGroupTemplate, BlockTemplate,
  IdentifyParameters, IdentifyTask,
  InfoTemplate, MarkerSymbol, FillSymbol,
  esriRequest, Query, QueryTask, esriConfig,
  d3
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
          name: fieldLabels[field],
          population: +attributes[field]
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
      var width = 240,
        height = 210,
        radius = Math.min(width, height) / 2;

      var color = d3.scale.ordinal()
        .range(["#98abc5", "#8a89a6", "#7b6888", "#6b486b", "#a05d56", "#d0743c", "#ff8c00", "#c7d223"]);

      var arc = d3.svg.arc()
        .outerRadius(radius - 10)
        .innerRadius(0);

      var pie = d3.layout.pie()
        .sort(null)
        .value(function(d) { return d.population; });

      var svg = d3.select(".censusethnic").append("svg")
        .attr("width", width)
        .attr("height", height)
        .append("g")
        .attr("transform", "translate(" + width / 2 + "," + height / 2 + ")");


      if (!data || !data.length) {
      	return;
      }

      var g = svg.selectAll(".arc")
        .data(pie(data))
        .enter().append("g")
        .attr("class", "arc");

      g.append("path")
        .attr("d", arc)
        .style("fill", function(d) { return color(d.data.name); });


      g.append("text")
        .attr("transform", function(d) { return "translate(" + arc.centroid(d) + ")"; })
        .attr("dy", ".35em")
        .style("text-anchor", "middle")
        .text(function(d) { return d.data.name; });

    },

    genderData: function (attributes) {
      var fieldLabels = {
        "MALES": "Males",
        "FEMALES": "Females"
      };

    	return this.formatAttributesForGraph(attributes, fieldLabels);
    },

    genderGraph: function (data) {
    	var width = 240,
        height = 210,
        radius = Math.min(width, height) / 2;

      var color = d3.scale.ordinal()
        .range(["#8888ff", "#ff8888"]);

      var arc = d3.svg.arc()
        .outerRadius(radius - 10)
        .innerRadius(0);

      var pie = d3.layout.pie()
        .sort(null)
        .value(function(d) { return d.population; });

      var svg = d3.select(".censusgender").append("svg")
        .attr("width", width)
        .attr("height", height)
        .append("g")
        .attr("transform", "translate(" + width / 2 + "," + height / 2 + ")");

      if (!data || !data.length) {
      	return;
      }

      var g = svg.selectAll(".arc")
        .data(pie(data))
        .enter().append("g")
        .attr("class", "arc");

      g.append("path")
        .attr("d", arc)
        .style("fill", function(d) { return color(d.data.name); });


      g.append("text")
        .attr("transform", function(d) { return "translate(" + arc.centroid(d) + ")"; })
        .attr("dy", ".35em")
        .style("text-anchor", "middle")
        .text(function(d) { return d.data.name; })
        .append("tspan")
        .text(function(d) { return d.data.population;})
        .attr("x", "0").attr("dy", '15');
    },

    ageData: function (attributes) {
      var fieldLabels = {
        "AGE_UNDER5": "< 5",
        "AGE_5_17": "5-17",
        "AGE_18_21": "18-21",
        "AGE_22_29": "22-29",
        "AGE_30_39": "30-39",
        "AGE_40_49": "40-49",
        "AGE_50_64": "50-64",
        "AGE_65_UP": "> 64"
      };

      return this.formatAttributesForGraph(attributes, fieldLabels);
    },
    ageGraph: function (data) {

      var maxData = d3.max(arrayUtils.map(data, function (item) {return item.population;}));

      var x = d3.scale.linear()
        .domain([0, maxData])
        .range([0, 240]);

      d3.select(".censusages")
        .selectAll("div")
        .data(data)
          .enter().append("div")
          .style("width", function(d) { return x(d.population) + "px"; })
          .text(function(d) { return d.name + ": " + d.population; });
    }

  });
});
