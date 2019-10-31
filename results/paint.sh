#! /bin/sh

echo Start painting rate...
Rscript rate-graph.R

echo Start painting gini...
Rscript gini-graph.R

echo Start painting gw...
Rscript gw.R

