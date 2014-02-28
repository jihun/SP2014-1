var express = require('express'),
  app = express(),
  swig = require('swig'),
  people;

// This is where all the magic happens!
app.engine('html', swig.renderFile);

app.set('view engine', 'html');
app.set('views', __dirname + '/views');
app.use(express.static(__dirname + '/'));

// Swig will cache templates for you, but you can disable
// that and use Express's caching instead, if you like:
app.set('view cache', false);
// To disable Swig's cache, do the following:
swig.setDefaults({ cache: false });
// NOTE: You should always cache templates in a production environment.
// Don't leave both of these to `false` in production!

app.get('/', function (req, res) {
  res.render('index.html', { /* template locals context */ });
});

app.get('/d3test', function (req, res) {
  res.render('d3test.html', { /* template locals context */ });
});

app.get('/start', function (req, res) {	
	console.log(req.query.string);

	fs = require('fs')
	fs.writeFileSync('test_str.txt', req.query.string, 'utf8');

	var execSync = require('exec-sync');
	execSync('./a.out');

	/*var FFI = require("node-ffi");
	var libc = new FFI.Library(null, {
	"system": ["int32", ["string"]]
	});

	var run = libc.system;
	run("./a.out < input.txt");*/

	var data = fs.readFileSync('result.txt', 'utf8');
	if(data.indexOf("YES") != -1)
	{
		//csv = fs.readFileSync('trace.csv', 'utf8');
		var Converter = require('csvtojson').core.Converter;
		var csvFileName = "trace.csv";
		var csvConverter = new Converter();

		csvConverter.on("end_parsed", function(jsonObj)
		{
			jsonObj["result"] = "YES";
			res.send(jsonObj);
		});
		csvConverter.from(csvFileName);
	}
	else
	{
		var jsonObj = {};
		jsonObj["result"] = "NO";
		res.send(jsonObj);
	}
});

app.get('/update', function (req, res) {

	for(i=0; i<100000000;i++)
	{
	}
	var FFI = require("node-ffi");
	var libc = new FFI.Library(null, {
	"system": ["int32", ["string"]]
	});

	var run = libc.system;
	run("./a.out");

	fs = require('fs')
	fs.readFile('output.txt', 'utf8', function (err,data) {
	if (err) {
	return console.log(err);
	}
		console.log(data);
	});

res.send('ok');
});

app.listen(1337);
console.log('Application Started on http://localhost:1337/');