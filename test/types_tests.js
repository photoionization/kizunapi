exports.runTests = (assert, binding) => {
  assert.equal(binding.value, 'value', 'ToNode value')
  assert.strictEqual(binding.null, null, 'ToNode null')
  assert.equal(binding.integer, 123, 'ToNode integer')
  assert.equal(binding.number, 3.14, 'ToNode number')
  assert.equal(binding.bool, false, 'ToNode bool')
  assert.equal(binding.string, '字符串', 'ToNode string')
  assert.equal(binding.ustring, 'ustring', 'ToNode ustring')
  assert.equal(binding.charptr, 'チャーポインター', 'ToNode charptr')
  assert.equal(binding.ucharptr, 'ucharptr', 'ToNode ucharptr')
  assert.equal(typeof binding.symbol, 'symbol', 'ToNode symbol')
  assert.deepStrictEqual(binding.tuple, [89, true, '64'], 'ToNode tuple')
  assert.deepStrictEqual(binding.pair, ['a', 'pair'], 'ToNode pair')
  assert.equal(binding.variant, 8964, 'ToNode variant')
  assert.deepStrictEqual(binding.map, {"123": 456}, 'ToNode map')
  assert.deepStrictEqual(binding.passTuple([89, 64]), [89, 64],
                         'FromNode tuple')
  assert.deepStrictEqual(binding.passPair([89, 64]), [89, 64],
                         'FromNode pair')
  assert.deepStrictEqual(binding.passVariant('str'), 'str',
                         'FromNode variant str')
  assert.equal(binding.passVariant(2), 2, 'FromNode variant number')
  assert.throws(() => binding.passVariant(false),
                /Error processing argument at index 0/,
                'FromNode variant throws')
  assert.deepStrictEqual(binding.passMap({'str': 123}), {'str': 123},
                         'FromNode map')
}
