//
// Created by matf-rg on 30.10.20..
//

#ifndef PROJECT_BASE_TEXTURE2D_H
#define PROJECT_BASE_TEXTURE2D_H
#include <glad/glad.h>
#include <stb_image.h>
#include <rg/Error.h>

class Texture2D {
public:
    unsigned m_tex;
    unsigned char * m_data;

    Texture2D(GLenum wrap_s, GLenum wrap_t, GLenum mag_filter, GLenum min_filter){
        glGenTextures(1,&m_tex);
        glBindTexture(GL_TEXTURE_2D, m_tex);

        //wrap
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);

        //filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    }

    void specify_texture_image(GLenum target,
                               GLint internalFormat,
                               GLsizei width,
                               GLsizei height,
                               GLenum format,
                               GLenum type,
                               GLint level = 0,
                               GLint border = 0,
                               const GLvoid * data = NULL){
        glTexImage2D(target, level, GL_DEPTH_COMPONENT, width, height, border, GL_DEPTH_COMPONENT, GL_FLOAT, data);
    }

    void attach_framebuffer(unsigned int framebuffer){
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_tex, 0);
        //We need to explicitly tell OpenGL that we won't render any color data
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    };

    void reflect_vertically(){
        stbi_set_flip_vertically_on_load(true);
    }

    void load(std::string path_to_img, GLenum rgb){
        int width, height, n_channels;

        m_data = stbi_load(path_to_img.c_str(), &width, &height, &n_channels, 0);

        if(m_data){
            glTexImage2D(GL_TEXTURE_2D, 0, rgb, width, height, 0, rgb, GL_UNSIGNED_BYTE, m_data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else{
            std::cerr << "Failed to load texture\n";
        }
    }

    void free_data(){
        stbi_image_free(m_data);
    }

    void activate(GLenum texture_number) {
        glActiveTexture(texture_number);
        glBindTexture(GL_TEXTURE_2D, m_tex);
    }
};


#endif //PROJECT_BASE_TEXTURE2D_H
