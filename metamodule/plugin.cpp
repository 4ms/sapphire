#include "plugin.hpp"

// Stubs for models that aren't in the MM plugin, but are used in common code
Model *modelSapphireTricorder = nullptr;
Model *modelSapphireChaops = nullptr;

#ifdef METAMODULE_BUILTIN
extern Plugin *pluginInstance;
__attribute__((visibility("default"))) void init_Sapphire(Plugin *p) {
#else
Plugin *pluginInstance;
__attribute__((visibility("default"))) void init(Plugin *p) {
#endif
	pluginInstance = p;

	p->addModel(modelSapphireElastika);
	p->addModel(modelSapphireFrolic);
	p->addModel(modelSapphireGalaxy);
	p->addModel(modelSapphireGlee);
	p->addModel(modelSapphireGravy);
	p->addModel(modelSapphireHiss);
	p->addModel(modelSapphireLark);
	// p->addModel(modelSapphireMoots);
	// p->addModel(modelSapphireNucleus);
	// p->addModel(modelSapphirePivot);
	// p->addModel(modelSapphirePolynucleus);
	// p->addModel(modelSapphirePop);
	// p->addModel(modelSapphireRotini);
	// p->addModel(modelSapphireSam);
	// p->addModel(modelSapphireTin);
	// p->addModel(modelSapphireTout);
	// p->addModel(modelSapphireTricorder);
	// p->addModel(modelSapphireTubeUnit);
}

extern "C" void _jp2uc_l() {
}
