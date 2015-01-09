'''
NPS Server for *nix
@summary: Net promoter score server for the guest satisfaction kiosk

@copyright: (c) 2015 LLPA. ALL RIGHTS RESERVED
@version: 1.0

@organization: LLPA
@author: gregory l. bodine
@created on Jan 4, 2015

@change: 
  2015-01-04 initial creation. 
  
@todo:

@note:
 
@license: 
  we need to create one.
'''
from gevent import monkey; monkey.patch_all()

import os
import sys
import datetime
import time
import logging
import json
import csv

from bottle import route, run, get, post, static_file, redirect, request, response, error


#-----------------------------------------------------------------------------------------------------------------------
# bottle routes
#-----------------------------------------------------------------------------------------------------------------------
@route('/')
@route('/index.html')
def index():
    raise static_file('index.html', root='./app')

@route('/favicon.ico')
def favicon():
    return static_file('favicon.ico', root='./app/')   

@error(404)
def mistake404(code):
    return static_file('404.html', root='./app')

@post('/api/datalogger')
def post():
    
    logging.info('in post for /api/datalogger')
    st = datetime.datetime.utcnow().isoformat() + 'Z'
    
    with open('cumulus.csv', 'a+') as csvfile:
        spamwriter = csv.writer(csvfile, delimiter=',',quotechar='|', quoting=csv.QUOTE_MINIMAL)
        
        csvfile.seek(0,2) # move the cursor to the end of the file
        size = csvfile.tell()
    
        if size == 0:      
            spamwriter.writerow(['timestamp', 'devicenumber', 'thumb'])
            
        spamwriter.writerow([st, '1111', 10])
        spamwriter.close()

    response.content_type = 'application/json' 
    return {'method called':  'success'}

#-----------------------------------------------------------------------------------------------------------------------
# main
# - start listening to incoming requests for analytics service
#-----------------------------------------------------------------------------------------------------------------------
def main():
    name = 'cumulusd'
    logpth = None
    loglvl = logging.INFO
    msgformat = '%(asctime)s ' + name + ' %(levelname)s:%(message)s'
    logging.basicConfig(format=msgformat, filename=logpth, level=loglvl)
    logging.info('Starting NPS Server')
    run(host='0.0.0.0', port=8080, server='gevent', debug=True, reloader=True)

if __name__ == '__main__':
    main() 
