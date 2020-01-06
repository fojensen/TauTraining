import json

data = {}
data['people'] = []

data['people'].append({
   'name': 'QCD_Flat',
   'das': '/QCD_Flat_Pt-15to7000_TuneCUETP8M1_14TeV_pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globalTag': '93X_upgrade2023_realistic_v5',
   'xs': '1.',
   'nEvents': '7072350',
   'nFiles': '803',
})
data['people'].append({
   'name': 'QCD_Flat_Run2',
   'das': '/QCD_Pt-15to7000_TuneCP5_Flat_13TeV_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1/MINIAODSIM',
   'globalTag': '102X_upgrade2018_realistic_v15',
   'xs': '1.',
   'nEvents': '19882000',
   'nFiles': 293,
})
data['people'].append({
   'name': 'WToLNu_2J',
   'das': '/WToLNu_2J_14TeV-madgraphMLM-pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globalTag': '93X_upgrade2023_realistic_v5',
   'xs': '3453.',
   'nEvents': '363169',
   'nFiles': '4',
})
data['people'].append({
   'name': 'DYToLL-M-50_2J',
   'das': '/DYToLL-M-50_2J_14TeV-madgraphMLM-pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globalTag': '93X_upgrade2023_realistic_v5',
   'xs': '363.5',
   'nEvents': '2002641',
   'nFiles': '181',
})
data['people'].append({
   'name': 'TTTo2L2Nu',
   'das': '/TTTo2L2Nu_TuneCUETP8M1_14TeV-powheg-pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globalTag': '93X_upgrade2023_realistic_v5',
   'xs': '90.75',
   'nEvents': '2866568',
   'nFiles': '321',
})
data['people'].append({
   'name': 'GluGluHToTauTau',
   'das': '/GluGluHToTauTau_M125_14TeV_powheg_pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globalTag': '93X_upgrade2023_realistic_v5',
   'xs': '50.35',
   'nEvents': '91120',
   'nFiles': '12',
})
#data['people'].append({
#   'name': "GluGluHToTauTau_Run2",
#   'das': '/GluGluHToTauTau_M125_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15-v2/MINIAODSIM',
#   'globalTag': '102X_upgrade2018_realistic_v15',
#   'xs': '28.0525',
#   'nEvents': '12575000',
#   'nFiles': '188',
#})
data['people'].append({
   'name': 'VBFHToTauTau',
   'das': '/VBFHToTauTau_M125_14TeV_powheg_pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globalTag': '93X_upgrade2023_realistic_v5',
   'xs': '4.172',
   'nEvents': '99000',
   'nFiles': '12',
})
data['people'].append({
   'name': 'VBFHToTauTau_Run2',
   'das' : '/VBFHToTauTau_M125_13TeV_powheg_pythia8/RunIIAutumn18MiniAOD-102X_upgrade2018_realistic_v15_ext1-v1/MINIAODSIM',
   'globalTag': '102X_upgrade2018_realistic_v15',
   'xs': '3.861',
   'nEvents': '3000000',
   'nFiles' : '72',
})

with open('mcSamples.json', 'w') as outfile:
   json.dump(data, outfile)
