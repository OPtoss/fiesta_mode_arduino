# Bootstrap the system
import sys
sys.path.insert(0, '/mnt/sda1/python-packages')
sys.path.insert(0, '/usr/lib/python2.7/bridge/')

# Import libs
from flask import Flask, render_template, request

foundBridge = False
try:
	from bridgeclient import BridgeClient as bridgeclient
	from tcp import TCPJSONClient
	foundBridge = True
except:
	print "Failed to import bridge library!"

# Init Bridge
if foundBridge:
	# Use tcp client directly to avoid bridge opening and closing sockets every request
	client = TCPJSONClient('127.0.0.1', 5700)
	#client = bridgeclient()

# Init Flask
app = Flask(__name__)
app.debug = True

class Modes:
	SOLID = "1"

# Begin App

@app.route('/')
def hello_world():
	return "Hello World!"


@app.route('/solid', methods=['GET', 'POST'])
def solid():
    if request.method == 'POST':
        return update_solid(
        	request.form['r'], 
        	request.form['g'], 
        	request.form['b']
        )
    else:
        return show_solid_form()

def update_solid(r, g, b):
	if foundBridge:
		client.send({'command':'put', 'key':'r', 'value':r})
		client.send({'command':'put', 'key':'g', 'value':g})
		client.send({'command':'put', 'key':'b', 'value':b})
		# client.put("mode", Modes.SOLID)
		# client.put("r", r)
		# client.put("g", g)
		# client.put("b", b)

	return "Set solid color to (%s,%s,%s)\n" % (r,g,b)

def show_solid_form():
	return render_template('color_from_image.html')



if __name__ == '__main__':
	app.run(host='0.0.0.0')