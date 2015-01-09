'''
NPS Server for Windows
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

import win32serviceutil
import win32service
import win32event
import servicemanager
import socket
import datetime
import time
import logging
import json
import csv

from bottle import route, run, get, post, static_file, redirect, request, response, error

#-----------------------------------------------------------------------------------------------------------------------
# Setup the logging facility
#-----------------------------------------------------------------------------------------------------------------------
logging.basicConfig(
    filename = None,
    level = logging.DEBUG, 
    format = '[cumulus-service] %(levelname)-7.7s %(message)s'
)


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
# The Service Control for the Cumulus windows service
#----------------------------------------------------------------------------------------------------------------------- 
class CumulusSvc (win32serviceutil.ServiceFramework):
    _svc_name_ = "Cumulus-Service"
    _svc_display_name_ = "Cumulus Service"
    
    def __init__(self,args):
        win32serviceutil.ServiceFramework.__init__(self,args)
        self.stop_event = win32event.CreateEvent(None,0,0,None)
        socket.setdefaulttimeout(60)
        self.stop_requested = False
 
    def SvcStop(self):
        self.ReportServiceStatus(win32service.SERVICE_STOP_PENDING)
        win32event.SetEvent(self.stop_event)
        logging.info('Stopping service ...')
        self.stop_requested = True
 
    def SvcDoRun(self):
        servicemanager.LogMsg(
            servicemanager.EVENTLOG_INFORMATION_TYPE,
            servicemanager.PYS_SERVICE_STARTED,
            (self._svc_name_,'')
        )
        self.main()
 
    def main(self):
        logging.info('Starting NPS Server')
        run(host='0.0.0.0', port=8080, server='gevent', debug=True, reloader=True) 
        return
 
if __name__ == '__main__':
    win32serviceutil.HandleCommandLine(CumulusSvc)
