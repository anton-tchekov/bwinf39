#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

/* See und Eisbuden als SVG ausgeben */
static void svg_cross(FILE *fp, float x, float y, float size, const char *color)
{
	fprintf(fp, "\t<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\" stroke=\"%s\" stroke-width=\"2\" />\n",
			(x - size),
			(y - size),
			(x + size),
			(y + size),
			color);

	fprintf(fp, "\t<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\" stroke=\"%s\" stroke-width=\"2\" />\n",
			(x - size),
			(y + size),
			(x + size),
			(y - size),
			color);
}

static void svg_export(FILE *fp, std::vector<int> const &addrs, int n, int result[3])
{
	const float cross_size = 8.0f, tick_size = 5.0f;
	const int radius = 400, radius2 = 420;
	const char red[] = "red";
	const char blue[] = "blue";
	const char green[] = "green";

	/* SVG */
	fprintf(fp,
			"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			"<svg xmlns=\"http://www.w3.org/2000/svg\"\n"
			"\txmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
			"\tversion=\"1.1\" baseProfile=\"full\"\n"
			"\twidth=\"100%%\" height=\"100%%\"\n"
			"\tviewBox=\"-500 -500 1000 1000\">\n"
			"\t<title>Eisbuden</title>\n"
			"\t<desc>Zeigt die Haeuser und Eisbuden um den See</desc>\n");

	/* See */
	fprintf(fp, "\t<circle cx=\"0\" cy=\"0\" r=\"%d\" fill=\"white\" stroke=\"black\" stroke-width=\"2\" />\n", radius);

	/* Adresse 0 */
	svg_cross(fp, 0, -radius2, cross_size, blue);

	/* Markierungen */
	for(int i = 0; i < n; ++i)
	{
		float theta, x, y;
		theta = ((float)i / (float)n) * 2.0f * M_PI - M_PI / 2.0f;
		x = cos(theta);
		y = sin(theta);
		fprintf(fp, "\t<line x1=\"%g\" y1=\"%g\" x2=\"%g\" y2=\"%g\" stroke=\"black\" stroke-width=\"2\" />\n",
				((radius - tick_size) * x),
				((radius - tick_size) * y),
				((radius + tick_size) * x),
				((radius + tick_size) * y));
	}

	/* Haeuser */
	for(int i = 0; i < (int)addrs.size(); ++i)
	{
		float theta, x, y;
		theta = ((float)addrs[i] / (float)n) * 2.0f * M_PI - M_PI / 2.0f;
		x = radius * cos(theta);
		y = radius * sin(theta);
		svg_cross(fp, x, y, cross_size, red);
	}

	/* Eisbude */
	for(int i = 0; i < 3; ++i)
	{
		float theta, x, y;
		theta = ((float)result[i] / (float)n) * 2.0f * M_PI - M_PI / 2.0f;
		x = radius2 * cos(theta);
		y = radius2 * sin(theta);
		svg_cross(fp, x, y, cross_size, green);
	}

	fprintf(fp, "</svg>\n");
}

int main(int argc, char **argv)
{
	/* Kommandozeilenargumente ueberpruefen */
	if(argc != 5)
	{
		printf("Usage: ./svgout filename e0 e1 e2\n");
		return 1;
	}

	/* Datei oeffnen */
	std::ifstream in(argv[1]);
	if(!in)
	{
		printf("File doesn't exist\n");
		return 1;
	}

	int n, m, e[3];
	e[0] = atoi(argv[2]);
	e[1] = atoi(argv[3]);
	e[2] = atoi(argv[4]);

	in >> n;
	in >> m;
	if(n <= 0 || m <= 0)
	{
		printf("Invalid input\n");
		return 1;
	}

	std::vector<int> addrs(m);
	for(int i = 0; i < m; ++i)
	{
		in >> addrs[i];
	}

	char buf[64];
	snprintf(buf, sizeof(buf), "out_%d_%d_%d.svg", e[0], e[1], e[2]);
	FILE *fp;
	if(!(fp = fopen(buf, "w")))
	{
		printf("Failed to write %s\n", buf);
		return 1;
	}

	svg_export(fp, addrs, n, e);
	fclose(fp);
	return 0;
}
