module.exports = function(grunt) {
    grunt.initConfig({
        intern: {
            dev: {
                options: {
                    runType: 'runner',
                    config: 'tests/intern'
                }
            }
        },
        esri_slurp: {
			options: {
				version: '3.13'
			},
            dev: {
                options: {
                    beautify: false
                },
				dest: 'esri'
            }
        },
		esri_slurp_modules:{
		  options: {
			version: '3.13',
			src: './',
			dest: './modules'
		  }
		},
        watch: {
            all: {
                options: { livereload: true },
                files: ['js/*.js']
            }
        }
    });

    // Loading using a local copy
    grunt.loadNpmTasks('intern');
    grunt.loadNpmTasks('grunt-contrib-watch');
    grunt.loadNpmTasks('grunt-esri-slurp');

    // download Esri JSAPI
    grunt.registerTask('slurp', ['esri_slurp']);

	grunt.registerTask('create_modules', ['esri_slurp_modules']);
	
    // Register a test task
    grunt.registerTask('test', ['intern']);

    // By default we just test
    grunt.registerTask('default', ['test']);
};