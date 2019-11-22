import json
import os

infile = 'mcSamples.json'

with open(infile) as json_file:

   data = json.load(json_file)
   
   for p in data['people']:
      f = open("./crabSubmits/"+p['name']+".py","w+")
      f.write("from CRABClient.UserUtilities import config, getUsernameFromSiteDB\n")
      f.write("config = config()\n")
      f.write("\n")
      f.write("config.General.requestName = 'TauTraining_"+p['name']+"'\n")
      f.write("config.General.workArea = 'crab_projects'\n")
      f.write("config.General.transferOutputs = True\n")
      f.write("config.General.transferLogs = True\n")
      f.write("\n")
      f.write("config.JobType.pluginName = 'Analysis'\n")
      f.write("config.JobType.psetName = './tauAnalyzer_cfg.py'\n")
      f.write("config.JobType.pyCfgParams = ['']\n")
      f.write("\n")
      f.write("config.Data.inputDataset = '"+p['das']+"'\n")
      f.write("config.Data.splitting = 'Automatic'\n")
      f.write("config.Data.outLFNDirBase = '/store/user/fjensen/TauTraining/'\n")
      f.write("\n")
      f.write("config.Site.storageSite = 'T3_US_FNALLPC'\n")
      f.close()
      # comment the next line for a dry run
      #os.system("crab submit " + f.name)

