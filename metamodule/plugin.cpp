#include "plugin.hpp"

Plugin *pluginInstance;

__attribute__((visibility("default"))) void init(Plugin *p) {
	pluginInstance = p;

	p->addModel(modelSapphireElastika);
	p->addModel(modelSapphireFrolic);
	p->addModel(modelSapphireGalaxy);
	p->addModel(modelSapphireGlee);
	p->addModel(modelSapphireGravy);
	p->addModel(modelSapphireHiss);
	p->addModel(modelSapphireLark);
	p->addModel(modelSapphireZoo);
	p->addModel(modelSapphireChaops);
	p->addModel(modelSapphireTin);
	p->addModel(modelSapphireTout);
	p->addModel(modelSapphireTricorder);
	// p->addModel(modelSapphireMoots);
	// p->addModel(modelSapphireNucleus);
	// p->addModel(modelSapphirePivot);
	// p->addModel(modelSapphirePolynucleus);
	// p->addModel(modelSapphirePop);
	// p->addModel(modelSapphireRotini);
	// p->addModel(modelSapphireSam);
	// p->addModel(modelSapphireTubeUnit);
}

extern "C" void _jp2uc_l() {
}
