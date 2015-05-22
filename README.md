1, Two secondary vertex producer

A: HeavyFlavorHLTHI/JetDmesonHLTHI/src/HLTDisplacedDitkVtxProducer.cc
Input collection is JetTracksAssociationCollection

B: HeavyFlavorHLTHI/JetDmesonHLTHI/src/HLTDisplacedtktkVtxProducer.cc
Input collection is reco::Track colleciton

2, Secondary vertex filter

HeavyFlavorHLTHI/JetDmesonHLTHI/src/HLTDisplacedDmesonVtxFilter.cc
Applying topological cuts on the secondary vertex.
Trigger objects have not been added yet.

3, In HeavyFlavorHLTHI/JetDmesonHLTHI/test/reRun_bJetHLT_cfg_test.py, you can see how the trigger paths are built presently.
