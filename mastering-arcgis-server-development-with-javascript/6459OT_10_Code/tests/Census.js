define([
    'intern!object',
    'intern/chai!assert',
    'app/Census',
	'esri/map',
	'dojo/dom-construct',
	'dojo/_base/window'
], function(registerSuite, assert, Census, Map, domConstruct, win) {
    var map, census;

	registerSuite({
        name: 'Create a Census Widget',
		// before the suite starts
        setup: function() {
            // create a map div in the body, load esri css, and create the map for our tests
            domConstruct.place('<link rel="stylesheet" type="text/css" href="//js.arcgis.com/3.13/dijit/themes/claro/claro.css">', win.doc.getElementsByTagName("head")[0], 'last');
            domConstruct.place('<link rel="stylesheet" type="text/css" href="//js.arcgis.com/3.13/esri/css/esri.css">', win.doc.getElementsByTagName("head")[0], 'last');
            domConstruct.place('<div id="map" style="width:300px;height:200px;" class="claro"><div id="census-widget"></div></div>', win.body(), 'only');

            map = new Map("map", {
                basemap: "topo",
                center: [-122.45, 37.75],
                zoom: 13,
                sliderStyle: "small"
            });

			census = new Census({
				map: map,
				mapService: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/"
			}, "census-widget");
        },

		 // before each test executes
        beforeEach: function() {
            // do nothing
        },

		afterEach: function () {
		},

        // after the suite is done (all tests)
        teardown: function() {
            map.destroy();
        },
        'Test Census widget creation': function() {
			assert.strictEqual(census.baseClass, "y2k-census", "census.baseClass should return a string 'y2k-census'");

        },

		'Test Census query for dropdown data': function () {

			census.queryShapeAndData({
				url: "http://sampleserver6.arcgisonline.com/arcgis/rest/services/Census/MapServer/3",
				fields: ["STATE_NAME", "STATE_FIPS"],
				where: "1=1",
			}).then(function (featureSet) {
				assert.strictEqual(featureSet.features.length, 51, "There should be 51 states returned, including the District of Columbia");
			});
		},

		'Test Census Graphing Attributes': function () {

			var ethnicAttributes = {
				WHITE: 10,
				BLACK: 20,
				AMERI_ES: 12,
    		ASIAN: 11,
				HAWN_PI: 4,
    		HISPANIC: 23,
    		OTHER: 7,
				MULT_RACE: 17
			};

			var data = census.ethnicData(ethnicAttributes);

			assert.strictEqual(data.length, 8, "The translation from graphic attributes to d3-based data should have 8 attributes in the ethnicData function");
			assert.strictEqual(data[4].name, "Hawaiian / Pacific Islander", "The item in index 4 should have a name of Hawaiian / Pacific Islander data");
			assert.strictEqual(data[5].population, 23, "Out of the Hispanic column, the data index of 5 should have a population of 23.");
		}
    });
});
