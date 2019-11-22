import json

data = {}
data['people'] = []

data['people'].append({
   'name': 'WToLNu_0J_PU200',
   'das': '/WToLNu_0J_14TeV-madgraphMLM-pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
})
data['people'].append({
   'name': 'WToLNu_0J_noPU',
   'das': '/WToLNu_0J_14TeV-madgraphMLM-pythia8/PhaseIISpr18AODMiniAOD-noPU_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
})
data['people'].append({
   'name': 'GluGluHToTauTau_PU200',
   'das': '/GluGluHToTauTau_M125_14TeV_powheg_pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
})
data['people'].append({
   'name': 'GluGluHToTauTau_noPU',
   'das': '/GluGluHToTauTau_M125_14TeV_powheg_pythia8/PhaseIISpr18AODMiniAOD-noPU_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
})
data['people'].append({
   'name': 'QCD_Flat_Pt-15to7000_PU200',
   'das': '/QCD_Flat_Pt-15to7000_TuneCUETP8M1_14TeV_pythia8/PhaseIISpr18AODMiniAOD-PU200_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
})
data['people'].append({
   'name': 'QCD_Flat_Pt-15to7000_noPU',
   'das': '/QCD_Flat_Pt-15to7000_TuneCUETP8M1_14TeV_pythia8/PhaseIISpr18AODMiniAOD-noPU_93X_upgrade2023_realistic_v5-v1/MINIAODSIM',
})

with open('mcSamples.json', 'w') as outfile:
   json.dump(data, outfile)
