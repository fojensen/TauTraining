# TauTraining

Code for TMVA tau ID training on Phase 2 simulation.

### References:
[arXiv:1510.07488: Reconstruction and identification of tau lepton decays to hadrons and tau neutrinos at CMS](https://arxiv.org/abs/1510.07488)
[arXiv:1809.02816: Performance of reconstruction and identification of tau leptons decaying to hadrons and tau neutrinos in pp collisions at sqrt(s) = 13 TeV](https://arxiv.org/abs/1809.02816)

## To install the package:

### Install stuff to calculate DeepTauv2p1
`https://twiki.cern.ch/CMSPublic/SWGuidePFTauID#Running_of_the_DeepTauIDs_ver_20` 
* `cmsrel CMSSW_10_2_16`
* `cd CMSSW_10_2_16/src/`
* `cmsenv`
* `git cms-merge-topic -u cms-tau-pog:CMSSW_10_2_X_tau-pog_DeepTau2017v2p1_nanoAOD`
* `scram b -j 4`

### Clone the repository
`git clone https://github.com/fojensen/TauTraining`  
`scram build`

### submit crab jobs

### produce skims
`root makeSkim.c+`

### plot the variables for training
`root makeVarPlots.c+`

### calculate and add pT weights to skims
`root ptetaweights.c+`
`root addWeights.c+`

### run the training
`root runTraining.c+`


