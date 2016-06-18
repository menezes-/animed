#pragma once

#include <GL/glew.h>
#include <memory>
#include "Light.hpp"

class SpotLight final : public Light {
    friend class Scene;

public:
    virtual void applyUniforms(Shader &shader) const override;

    /**
     * A razão para passarmos um "valor cosseno" (e não o angulo do cutoff) é que, no fragment shader, estamos calculando o produto escalar entre
     * a direção da luz e a direção para qual o spot esta "olhando" assim o valor retornado é o cosseno entre os dois vetores
     * de modo que não podemos comparar diretamente um ângulo qualquer com esse valor (lembre da imagem da função seno).
     * Para recuperamos o "ângulo certo" teriamos que, então, calcular o coseno do inverso do produto escalar¹ entre os dois vetores,
     * e calcular o inverso é uma operação custosa pra se fazer na GPU. Então para aumentar, nem que um pouco, a performance
     * é calculado o conseno do angulo passado. Assim os dois angulos são representados como cosenos e podemos fazer a comparação
     * sem nenhuma operação cara.
     *
     * ¹= A.B = |A|.|B|.cos(θ)
     * @param cutOffCosine "valor cosseno"
     */
    void setCutOffAngle(GLfloat cutOff);

    void setOuterCutOffAngle(GLfloat outerCutOff);

    bool isSingleLight() const;

    /**
     * se for verdadeiro assume que não existe um array de spotLights e sim apenas um spotlight (no shader)
     * @param singleLight
     */
    void setSingleLight(bool singleLight);


protected:
    void initDefaults() override;

private:
    std::string lightContainer{"spotLights"};

    SpotLight(glm::vec3 position, glm::vec3 direction, int id);

    static std::unique_ptr<SpotLight> create(glm::vec3 position, glm::vec3 direction, int id);

    glm::vec3 direction;

    /*
     * material de referencia: http://learnopengl.com/#!Lighting/Light-casters
     * cutOffCosine:
     * eu não implementei os campos abaixo sozinho, tive que pesquisar e usei o algoritmo que eu achei.
     * Inicialmente era passado um angulo Phi, que era angulo que especificava o raio do spotlight, e um angulo theta
     * que era um angulo entre a direção da luz e a posição da luz. Porém definir um cutOffCosine permite uma otimização
     * essa otimização está explicada na documentação da função setCutOffAngle.
     *
     * outerCutOffCosine:
     * Sem isso o cone de luz fica muito  ""certinho"" nas bordas, isso suaviza as bordas da luz dando um efeito mais realista.
     * A ideia básica é criar um cone de luz "por fora" do cone de luz inicial que vai gradualmente diminuindo a iluminação
     * a equação implementada foi:
     * I=(θ−γ)/ϵ
     * onde: ϵ (epsilon) é a diferença dos cossenos entre o cone interior (ϕ) e o cone exterior(γ), ou seja, ϵ=ϕ−γ
     * isso gera I que representa a intensidade da luz no fragmento atual.
     *
     * A tabela abaixo, também tirada do material de referência, mostra alguns valores depois de aplicado a formula descrita acima
     * facilitando o entendimento do algoritmo
     *
     * PS: a implementação em si está nos fragment shaders
     *
+-------+-------------+-------------------+-------------+-------------------+-------------+------------------------+-------------------------------+
|   θ   | θ(em graus) | ϕ(cutoff interno) | ϕ(em graus) | γ(cutoff externo) | γ(em graus) |           ϵ            |               I               |
+-------+-------------+-------------------+-------------+-------------------+-------------+------------------------+-------------------------------+
| 0.87  |          30 | 0.91              | 25          | 0.82              | 35          | 0.91 - 0.82 = 0.09     | 0.87 - 0.82 / 0.09 = 0.56     |
| 0.9   |          26 | 0.91              | 25          | 0.82              | 35          | 0.91 - 0.82 = 0.09     | 0.9 - 0.82 / 0.09 = 0.89      |
| 0.97  |          14 | 0.91              | 25          | 0.82              | 35          | 0.91 - 0.82 = 0.09     | 0.97 - 0.82 / 0.09 = 1.67     |
| 0.83  |          34 | 0.91              | 25          | 0.82              | 35          | 0.91 - 0.82 = 0.09     | 0.83 - 0.82 / 0.09 = 0.11     |
| 0.64  |          50 | 0.91              | 25          | 0.82              | 35          | 0.91 - 0.82 = 0.09     | 0.64 - 0.82 / 0.09 = -2.0     |
| 0.966 |          15 | 0.9978            | 12.5        | 0.953             | 17.5        | 0.966 - 0.953 = 0.0448 | 0.966 - 0.953 / 0.0448 = 0.29 |
+-------+-------------+-------------------+-------------+-------------------+-------------+------------------------+-------------------------------+     */

    GLfloat cutOffCosine;
    GLfloat outerCutOffCosine;

    bool singleLight;

};

