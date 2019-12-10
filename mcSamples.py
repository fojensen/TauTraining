import json

data = {}
data['people'] = []

data['people'].append({
   'name': 'GluGluHToTauTau_PU200',
   'das': '/GluGluHToTauTau_M125_14TeV_powheg_pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '50.35',
   'nEvents': '91120',
   'nFiles': '12',
})
data['people'].append({
   'name': 'VBFHToTauTau_PU200',
   'das': '/VBFHToTauTau_M125_14TeV_powheg_pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '4.172',
   'nEvents': '99000',
   'nFiles': '12',
})
#data['people'].append({
#   'name': 'TTTo2L2Nu_PU200',
#   'das': '/TTTo2L2Nu_TuneCUETP8M1_14TeV-powheg-pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
#   'globaltag': '93X_upgrade2023_realistic_v5',
#   'xs': '90.75',
#   'nEvents': '2866568',
#   'nFiles': '321',
#})
data['people'].append({
   'name': 'TT_PU200',
   'das': '/TT_TuneCUETP8M2T4_14TeV-powheg-pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v2/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '864.4',
   'nEvents': '2869776',
   'nFiles': '331',
})
data['people'].append({
   'name': 'TT_PU200_ext',
   'das' : '/TT_TuneCUETP8M2T4_14TeV-powheg-pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5_ext1-v2/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '864.4',
   'nEvents': '1748600',
   'nFiles': '213',
})
data['people'].append({
   'name': 'QCD_Flat_Pt-15to7000_PU200',
   'das': '/QCD_Flat_Pt-15to7000_TuneCUETP8M1_14TeV_pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
   'globaltag': '93X_upgrade2023_realistic_v5',
   'xs': '1.',
   'nEvents': '7072350',
   'nFiles': '803'
})

with open('mcSamples.json', 'w') as outfile:
   json.dump(data, outfile)
