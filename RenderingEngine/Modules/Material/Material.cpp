#include "Material.h"
#include <iostream>
void Material::Bind(Shader* sh) const {
	if (diffuseTex) {
		diffuseTex->Bind(0);
		diffuseTex->SetScalingMode(texScalingMode, texScalingMode);
		sh->SetUniform1i("diffuseTex", 0);
	}
	
	sh->SetUniform4f("mat.diffuse", diffuse);
	sh->SetUniform1i("useDiffuseTex", diffuseTex != 0);

	if (ambientTex) {
		ambientTex->Bind(1);
		ambientTex->SetScalingMode(texScalingMode, texScalingMode);
		sh->SetUniform1i("ambientTex", 1);
	}
	
	sh->SetUniform3f("mat.ambient", ambient);
	sh->SetUniform1i("useAmbientTex", ambientTex != 0);

	if (specularTex) {
		specularTex->Bind(2);
		specularTex->SetScalingMode(texScalingMode, texScalingMode);
		sh->SetUniform1i("specularTex", 2);
	}
	sh->SetUniform3f("mat.specular", specular);
	sh->SetUniform1i("useSpecularTex", specularTex != 0);


	// UV tiling
	mat3 uvRotMat = rotate(mat4(1), texAngle, { 0,0,1 });
	mat3 uvScaleMat = mat2(texScale.x, 0, 0, texScale.y);
	uvScaleMat[2][2] = 1;
	mat3 uvTransMat = mat3(1);
	uvTransMat[2][0] = texOffset.x;
	uvTransMat[2][1] = texOffset.y;
	sh->SetUniformMat3f("mat.texTileMat", uvTransMat*uvScaleMat*uvRotMat);
	// Shiny
	sh->SetUniform1f("mat.shininess", shininess);
}
