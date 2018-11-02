const ciphers = [
	'9ecab968a638fae25210cf17bf1406e9',
	'3d3610cec6c3859656d528e6ec5344fa',
	'6e272243b978d0f8865eb5d3ac4f0ef8',
	'8cec5af6b8065fff5dbb3e6b24f1d503',
	'f1fca15dc89e7392d79ce8c31ca3e6f2',
	'e96f72361507e426f2c241401c2f700e',
	'4445940be9ecded4c6f753fba0549c99',
	'a164fe821203eed36317c44c69d4edc1',
	'26f3d61cdd0b73c13f83869dd62de982',
	'434ae964d73fabb3297604f0163d4618',
];

for (const cipher of ciphers) {
	const words = [];

	const ij = (i, j) => (
		cipher.slice((j * 4 + i) * 2, (j * 4 + i) * 2 + 2)
	);

	for (const i of Array(4).keys()) {
		words.push(ij(0, i) + ij(1, (i + 3) % 4) + ij(2, (i + 2) % 4) + ij(3, (i + 1) % 4));
	}

	console.log(`[${words.map((word) => `0x${word}`).join(', ')}],`);
}
