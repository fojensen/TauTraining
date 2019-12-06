import json

data = {}
data['people'] = []

data['people'].append({
   'name': 'GluGluHToTauTau_PU200',
   'das': '/GluGluHToTauTau_M125_14TeV_powheg_pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '50.35',
   'nevents': '91120'
})
data['people'].append({
   'name': 'VBFHToTauTau_PU200',
   'das': '/VBFHToTauTau_M125_14TeV_powheg_pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '4.172',
   'nevents': '99000',
})
data['people'].append({
   'name': 'TTTo2L2Nu_PU200',
   'das': '/TTTo2L2Nu_TuneCUETP8M1_14TeV-powheg-pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '90.75',
   'nevents': '1'
})
data['people'].append({
   'name': 'QCD_Flat_Pt-15to7000_PU200',
   'das': '/QCD_Flat_Pt-15to7000_TuneCUETP8M1_14TeV_pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '1',
   'nevents': '1'
})

with open('mcSamples.json', 'w') as outfile:
   json.dump(data, outfile)
