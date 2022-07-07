const fs = require('fs')
const path = require('path')
const assert = require('assert-simple-tap')

const bindings = require('./build/Debug/nb_tests')

for (const f of fs.readdirSync(__dirname)) {
  if (!f.endsWith('_tests.js'))
    continue
  const test = path.basename(f, '_tests.js')
  require(path.join(__dirname, f)).runTests(assert, bindings[test])
}