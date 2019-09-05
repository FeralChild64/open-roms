// Skip the gap between $C000-$DFFF

	.assert "Program Counter - C000", *, $C000
	.fill $2000, $00
	.assert "Program Counter - E000", *, $E000
