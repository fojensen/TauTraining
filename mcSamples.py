import json

data = {}
data['people'] = []

data['people'].append({
   'name': 'QCD_Flat_Pt-15to7000_PU200',
   'das': '/QCD_Flat_Pt-15to7000_TuneCUETP8M1_14TeV_pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '1.',
   'nEvents': '7072350',
   'nFiles': '803',
})
data['people'].append({
   'name': 'WToLNu_2J',
   'das': '/WToLNu_2J_14TeV-madgraphMLM-pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '3453.',
   'nEvents': '363169',
   'nFiles': '4',
})
data['people'].append({
   'name': 'DYToLL-M-50_2J',
   'das': '/DYToLL-M-50_2J_14TeV-madgraphMLM-pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '363.5',
   'nEvents': '2002641',
   'nFiles': '181',
})
data['people'].append({
   'name': 'TTTo2L2Nu',
   'das': '/TTTo2L2Nu_TuneCUETP8M1_14TeV-powheg-pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '90.75',
   'nEvents': '2866568',
   'nFiles': '321',
})
data['people'].append({
   'name': 'GluGluHToTauTau',
   'das': '/GluGluHToTauTau_M125_14TeV_powheg_pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '50.35',
   'nEvents': '91120',
   'nFiles': '12',
})
data['people'].append({
   'name': 'VBFHToTauTau',
   'das': '/VBFHToTauTau_M125_14TeV_powheg_pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '4.172',
   'nEvents': '99000',
   'nFiles': '12',
})

with open('mcSamples.json', 'w') as outfile:
   json.dump(data, outfile)
