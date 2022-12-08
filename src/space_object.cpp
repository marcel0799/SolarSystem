#include "space_object.h"

void Space_Object::update()
{
    /** \todo Update `model_matrix_` and position (`position_`) for each object.
    * 1. Combine translation and scaling matrices to get a result like in screenshots/planet_system_initial.png assuming:
    *   - sun at origin
    *   - all objects scaled by radius
    *   - all objects translated by `distance` in x direction
    * 2. Now use y-axis-rotation matrices in the right places to allow rotation
    *    around the object's axis (angle_self) and sun (angle_parent)
    * 3. You can also support a tilt angle if you like (`angle_tilt`) rotations
    * 4. Update the object's position using your constructed model matrix
    * Hints:
    *   - See glmath.h/cpp for an overview about implemented matrices.
    *   - Order is important!
    */

    model_matrix_ = mat4::rotate_y(angle_parent_) * //rotation um vorgesetzten (meistens Sonne)
                mat4::translate(vec3(distance_, 0.0, 0.0)) * //bewegt den planeten in einen distanc_ abstand zum vorgesetzten (meistens Sonne)
                mat4::scale(radius_) * //wir vergroessern den Planeten auf seine vorgegebene groesse
                mat4::rotate_x(angle_tilt_) * //verschiebung in x richtung zu irgendeinem abstand zur y achse
                mat4::rotate_y(angle_self_); //eigene dreh Bewegung, deswegen auch als erstes ausgefuehrt

                                //   0.0f bedeutet 0.0 nicht als double (standard) sondern als float
    position_ = model_matrix_ * vec4(0.0f, 0.0f, 0.0f, 1.0f); //setzt den planeten auf den ursprung und modelliert ihn dann

}

//-----------------------------------------------------------------------------

void Moon::update()
{
    /** \todo Update moon's `model_matrix_` and position (`position_`).
    *  The moon is a bit special, it must rotate around its `parent_planet_`. Be careful with the
    *  translation-rotation-order since rotation matrices always rotate around the current origin.
    */

    // wir merken und die position des parent planeten durch einen zugriff auf das Attribut position_ von dem parentplanet
    // parent_planet_ -> position_ == (parent_planet_*).position_ 
    // so greift man auf attribute von pointern zu
    vec4 parent_position = parent_planet_ -> position_;

    model_matrix_ = mat4::rotate_y(parent_planet_ -> angle_parent_) * // hinzufuegen der Rotation um die Sonne
                    mat4::translate(vec3(parent_planet_ -> distance_, 0.0, 0.0)) * //verschiebung zu dem Planeten um den Abstand zur Sonne
                    mat4::rotate_y(angle_parent_) *         // hinzufuegen der Rotation um den eltern Planeten 
                    mat4::translate(vec3(distance_,0,0)) *  // verschiebung um abstand zum eltern Planeten
                    mat4::scale(radius_) *          //vergroesserung auf die vorgeschriebene groesse des Mondes
                    mat4::rotate_x(angle_tilt_) *   //?? immernoch irgendein abstand zur y achse
                    mat4::rotate_y(angle_self_);    //zu beginn hat jeder Mond den Ursprung als Pos, dort soll er sich um sich selber drehen

    position_ = model_matrix_ * vec4(0.0, 0.0, 0.0, 1.0); // position ist auch hier ausgehend vom ursprung in kombi mit der model_matrix

}
//-----------------------------------------------------------------------------

void Space_Object::create_glow(float outer_radius, vec3 color)
{
    // create Billboard
    glow_ = new Billboard;

    // initialize texture layout
    glow_->texture_.init(GL_TEXTURE0, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR,
                         GL_LINEAR, GL_REPEAT);

    // create texture image
    int res = 1024;
    glow_->create_glow_texture(res, (int)(res / (2.0 * outer_radius)) - 1,
                               res / 2 - 1, color);

    // initialize quad vertex layouts
    glow_->init_GL_arrays();
}

//-----------------------------------------------------------------------------

void Space_Object::update_glow_angles(vec3 eye)
{
    if (glow_ != nullptr)
    {
        vec3 pos_to_eye = normalize(vec3(eye) - vec3(position_));
        glow_->update_angles(pos_to_eye);
    }
}

//=============================================================================
