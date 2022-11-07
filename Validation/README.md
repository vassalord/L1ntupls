# Setup
These scripts use the L1Ntuple framework, which should be set up as described here:

[https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TStage2Instructions#Environment_Setup_with_Integrati](https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TStage2Instructions#Environment_Setup_with_Integrati)

After setting up the L1Ntuple environment, issue the following:
```
git clone git@github.com:cms-hcal-trigger/Validation.git HcalTrigger/Validation
```

# HCAL conditions validation 
Scripts for HCAL radiation damage correction validation.

The script that submit CRAB jobs is called `submit_jobs.py`. Its required arguments are a good run lumimask, a dataset name, the new HcalL1TriggerObjects tag, and the storage site for the output. For example:
```
./submit_jobs.py -l lumimask_302472.json -d /ZeroBias/Run2017D-v1/RAW -t HcalL1TriggerObjects_2017Plan1_v13.0 -o T2_CH_CERN
```
Follow this twiki on how to use the Rates scripts:
https://twiki.cern.ch/twiki/bin/view/Sandbox/L1TriggerAtHCALdays2019#HCAL_conditions_impact_at_L1_rat


# HCAL Trigger studies for LLPs
Towards a new L1 seed to trigger on LLP signatures with HCAL using H/E + depth 

Start with L1Ntuple framework, and setup the L1T environment as here:

https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TStage2Instructions#Environment_Setup_with_Integrati

In 11_0_X:
```
#git cms-merge-topic --unsafe georgia14:upgradeHcalTPs-l1t-110X
#git cms-merge-topic --unsafe lwang046:upgradeHcalTPs-timeingbit-106X 
git cms-merge-topic --unsafe lwang046:upgradeHcalTPs-timeingbit-110X
git clone git@github.com:cms-hcal-trigger/validation.git HcalTrigger/Validation 
```

Example of energy depth profiles of HCAL TPs is shown here: 

https://github.com/gk199/Validation/blob/TimingAndDepth/bin/rates.cxx#L1168-L1189 

H/E on L1Jets can be applied as here:

https://github.com/gk199/Validation/blob/HoE_RatesWork/bin/rates.cxx#L373-L434
