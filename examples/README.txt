Directory contains testing files. Those contain:

    Normal cases:
- anchors.yaml: contains use of aliases with anchors
- anchorscomplex.yaml: more complex cases with multi-level anchors
- global-tag.yaml: test the use of a global tag
- mapping.yaml: test use of maps
- tags.yaml: test the use of tags
- array.yaml: test use of array
- json.yaml: test use of json file (Sequence with map too)
- numbers.yaml: Sequence of numbers (output similar to array.yaml)
- strings.yaml: Sequence with differently formated strings
- yaml-version.yaml: Sequence and map with yaml version
- multimap.yaml: Example for multiple maps
- multisequence.yaml: Example for multiple sequences inside a sequence

    Error cases:
- anchors2.yaml: anchors with an alias without an ancho
- mapseparatevalues.yaml -> 4ABK
- lineasmap.yaml
- errormapping.yaml
- unicodeanchor.yaml (libyaml error)

- "yaml-cpp needs to parse full doc" error

enddoc.yaml ->3HFZ (end doc error)

enddocandmore.yaml 

tagsrootobject.yaml  -> 35KP (solved)

-

specificnonspecifictag.yaml (tag error)



IMPORTANT TEMPORARY ERROR INFO:


-anchor as key

anchorwithcoloninname.yaml  -> 2SXE (weird anchor)

complexsequence.yaml (weird anchor) (solved)

anchorsandtokens.yaml (weird anchor)

propertyindicators.yaml

-

explicitemptykey.yaml (weird tag+weird anchor)

twoends.yaml (end weird)

- (empty key)

mapnoend.yaml (yaml-cpp refuse) (empty key)

mapwithempty.yaml (yaml-cpp refuse) (empty key)

explicitblockmapandemptycase.yaml (? tag error)
-

tagdeclarationbefiredoc.yaml

sequencescallar.yaml

tagshorthandfull.yaml 

tagopenend.yaml

anchorkeyandvalue.yaml

return at E76Z.tml

secondarytaghandle

CANT BE TESTED:
1)

- &a
- a
-
  &a : a
  b: &b
-
  &c : &a
-
  ? &d
-
  ? &e
  : &a

2)

---
key: &an:chor value

3)
&ee unicode anchor