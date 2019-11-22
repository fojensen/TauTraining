# TauTraining

Code for TMVA tau ID training on Phase 2 simulation.

### References:
[Reconstruction and identification of tau lepton decays to hadrons and tau neutrino at CMS](https://arxiv.org/abs/1510.07488)

## To install the package:

### Install stuff to calculate DeepTauv2p1
`https://twiki.cern.ch/CMSPublic/SWGuidePFTauID#Running_of_the_DeepTauIDs_ver_20` 
* `cmsrel CMSSW_10_2_16`
* `cd CMSSW_10_2_16/src/`
* `cmsenv`
* `git cms-merge-topic -u cms-tau-pog:CMSSW_10_2_X_tau-pog_DeepTau2017v2p1_nanoAOD`
* `scram b -j 4.
