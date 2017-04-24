//skybox

#include "common.h"

GLFWwindow* window;

int faceNumber;

float verticalAngle = -2.5f;
float horizontalAngle = 0.5f;
float initialFoV = 45.0f;
float speed = 5.0f;
float mouseSpeed = 0.005f;
float farPlane = 2000.f;

vec3 eyePoint = vec3( 12.f, 13.f, 1.f );
vec3 eyeDirection = vec3(
    sin(verticalAngle) * cos(horizontalAngle),
    cos(verticalAngle),
    sin(verticalAngle) * sin(horizontalAngle)
);
vec3 up = vec3( 0.f, 1.f, 0.f );

float verticalAngle2;// = verticalAngle - 3.14f;
vec3 eyePoint2;// = vec3(eyePoint.x, -eyePoint.y, eyePoint.z);
vec3 eyeDirection2;
/* = vec3(
    sin(verticalAngle2) * cos(horizontalAngle),
    cos(verticalAngle2),
    sin(verticalAngle2) * sin(horizontalAngle)
);*/

vec3 lightPosition = vec3( 3.f, 3.f, 3.f );
vec3 lightColor = vec3( 1.f, 1.f, 1.f );
float lightPower = 12.f;

vec3 materialDiffuseColor = vec3( 0.1f, 0.1f, 0.1f );
vec3 materialAmbientColor = vec3( 0.1f, 0.1f, 0.1f );
vec3 materialSpecularColor = vec3( 1.f, 1.f, 1.f );

const float SKYBOX_SIZE = 500.f;
GLfloat skyboxVertices[] = {
    // Positions
    -SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
    -SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
    -SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,

    -SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
    -SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    -SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
    -SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
    -SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
    -SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,

    SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
    SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
    SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
    SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,

    -SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
    -SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
    SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
    SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
    -SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,

    -SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
    SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
    -SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
    -SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,

    -SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    -SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
    -SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
    SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE
};

const float WATER_SIZE = 5.f;
const float WATER_Y = 2.2f;
GLfloat waterVertices[] = {
    -WATER_SIZE, WATER_Y, -WATER_SIZE,
    -WATER_SIZE, WATER_Y, WATER_SIZE,
    WATER_SIZE, WATER_Y, WATER_SIZE,
    WATER_SIZE, WATER_Y, WATER_SIZE,
    WATER_SIZE, WATER_Y, -WATER_SIZE,
    -WATER_SIZE, WATER_Y, -WATER_SIZE
};

GLfloat vertices_subscreen1[] = {
    //vertex coords
    -0.1f, 0.3f, -1,
    -0.1f, 0.9f, -1,
    -0.9f, 0.9f, -1,
    -0.9f, 0.9f, -1,
    -0.9f, 0.3f, -1,
    -0.1f, 0.3f, -1,

    //texture coords
    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f
};

GLfloat vertices_subscreen2[] = {
    //vertex coords
    0.1f, 0.3f, -1,
    0.9f, 0.3f, -1,
    0.1f, 0.9f, -1,
    0.1f, 0.9f, -1,
    0.9f, 0.3f, -1,
    0.9f, 0.9f, -1,

    //texture coords
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
};

GLuint vbo_skybox, obj_skybox_tex, obj_pool_tex,
    obj_subscreen1_tex, vbo_subscreen1,
    obj_subscreen2_tex, vbo_subscreen2;
GLuint vbo_model, vbo_model_normal;
GLuint vbo_water;
GLuint vao_skybox, vao_model, vao_water, vao_subscreen1, vao_subscreen2;
GLuint fbo_subscreen1, fbo_subscreen2;
GLuint subscreen1_depth, subscreen2_depth;
GLint uniform_model_skybox, uniform_view_skybox, uniform_projection_skybox;
GLint uniform_model_model, uniform_view_model, uniform_projection_model;
GLint uniform_model_water, uniform_view_water, uniform_projection_water;
GLint uniform_lightColor, uniform_lightPosition, uniform_lightPower, uniform_lightDirection;
GLint uniform_diffuseColor, uniform_ambientColor, uniform_specularColor;
GLint uniform_tex, uniform_tex_subscreen1, uniform_tex_subscreen2;
/*
mat4 ori_model_skybox, model_skybox, view_skybox, projection_skybox;
mat4 model_water, view_water, projection_water;
mat4 model_model, view_model, projection_model;
*/
mat4 ori_model_main, model_main, view_main, projection_main;
mat4 ori_model_sub2, model_sub2, view_sub2, projection_sub2;
mat4 model_skybox;
GLuint program_skybox, program_model, program_water,
    program_subscreen1, program_subscreen2;

void computeMatricesFromInputs();
void keyCallback( GLFWwindow*, int, int, int, int );
GLuint loadCubemap(vector<string>&);
void init_skybox();
void init_3d_model();
void init_water();
void init_subscreen1();
void init_subscreen2();
void drawSkybox(mat4& M, mat4& V, mat4& P);
void drawModels(mat4& M, mat4& V, mat4& P);
void drawWater(mat4& M, mat4& V, mat4& P);
void drawSubscreen1();
void drawSubscreen2();

int main(int argc, char** argv){
    // Initialise GLFW
	if(!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

    //without setting GLFW_CONTEXT_VERSION_MAJOR and _MINOR，
	//OpenGL 1.x will be used
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //must be used if OpenGL version >= 3.0
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Open a window and create its OpenGL context
	window = glfwCreateWindow(
        WINDOW_WIDTH, WINDOW_HEIGHT,
        "GLFW window with AntTweakBar",
        NULL, NULL
    );

	if(window == NULL){
		std::cout << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, keyCallback);

    /* Initialize GLEW */
    //without this, glGenVertexArrays will report ERROR!
    glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

    glEnable( GL_CULL_FACE );
    glEnable( GL_DEPTH_TEST );//must enable depth test!!

    //FreeImage library
    FreeImage_Initialise(true);

    model_main = translate( mat4( 1.f ), vec3( 0.f, 0.f, -4.f ) );
    ori_model_main = model_main;
    view_main = lookAt( eyePoint, eyePoint + eyeDirection, up );
    projection_main = perspective(
        initialFoV, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 0.01f, farPlane
    );

    model_sub2 = model_main;
    ori_model_sub2 = model_main;
    view_sub2 = lookAt(eyePoint2, eyePoint2 + eyeDirection2, up);
    projection_sub2 = perspective(
        initialFoV, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 0.01f, farPlane
    );

    init_skybox();
    init_3d_model();
    init_water();
    init_subscreen1();
    init_subscreen2();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor( 97/256.f, 175/256.f, 239/256.f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        computeMatricesFromInputs();

        //render to fbo_subscreen1
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_subscreen1);
        glClearColor( 171/256.f, 178/256.f, 191/256.f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        drawSkybox(model_skybox, view_main, projection_main);
        drawModels(model_main, view_main, projection_main);

        //render to fbo_subscreen2
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_subscreen2);
        glClearColor( 171/256.f, 178/256.f, 191/256.f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        drawSkybox(model_skybox, view_sub2, projection_sub2);
        drawModels(model_sub2, view_sub2, projection_sub2);

        //render to main screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        drawSkybox(model_skybox, view_main, projection_main);
        drawModels(model_main, view_main, projection_main);
        drawWater(model_main, view_main, projection_main);
        drawSubscreen1();
        drawSubscreen2();

        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    //FreeImage library
    FreeImage_DeInitialise();

    return EXIT_SUCCESS;
}

void computeMatricesFromInputs(){
	// glfwGetTime is called only once, the first time this function is called
	static float lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	float currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos( window, &xpos, &ypos );

	// Reset mouse position for next frame
	glfwSetCursorPos( window, WINDOW_WIDTH/2, WINDOW_HEIGHT/2 );

	// Compute new orientation
    //因为事先一步固定光标在屏幕中心
    //所以 WINDOW_WIDTH/2.f - xpos 和 WINDOW_HEIGHT/2.f - ypos 成了移动量
	horizontalAngle += mouseSpeed * float( xpos - WINDOW_WIDTH/2.f );
	verticalAngle += mouseSpeed * float( - ypos + WINDOW_HEIGHT/2.f );

    verticalAngle2 = abs(verticalAngle) - 3.14f;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	vec3 direction = vec3(
		sin(verticalAngle) * cos(horizontalAngle),
		cos(verticalAngle),
		sin(verticalAngle) * sin(horizontalAngle)
	);

    vec3 direction2 = vec3(
        sin(verticalAngle2) * cos(horizontalAngle),
		cos(verticalAngle2),
		sin(verticalAngle2) * sin(horizontalAngle)
    );

	// Right vector
	vec3 right = vec3(
        cos(horizontalAngle - 3.14/2.f), 0.f, sin(horizontalAngle - 3.14/2.f)
    );

	//new up vector
	vec3 newUp = cross( right, direction );
    vec3 newUp2 = cross( right, direction2 );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		eyePoint += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
        eyePoint -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
        eyePoint += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
        eyePoint -= right * deltaTime * speed;
	}

    eyePoint2 = vec3(eyePoint.x, -eyePoint.y, eyePoint.z);

    view_main = lookAt(eyePoint, eyePoint + direction, newUp);
    projection_main = perspective(initialFoV, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, farPlane);

    view_sub2 = lookAt(eyePoint2, eyePoint2 + direction2, newUp2);
    projection_sub2 = perspective(initialFoV, 1.f * WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, farPlane);

    //使 skybox 的中心永远位于 eyePoint
    //注意：GLM 的矩阵是 column major
    model_skybox[3][0] = ori_model_main[0][3] + eyePoint.x;
    model_skybox[3][1] = ori_model_main[1][3] + eyePoint.y;
    model_skybox[3][2] = ori_model_main[2][3] + eyePoint.z;
    glUniformMatrix4fv( uniform_model_skybox, 1, GL_FALSE,value_ptr(model_skybox));

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

void keyCallback(
    GLFWwindow* keyWnd,
    int key, int scancode, int action, int mods
){
    if(action == GLFW_PRESS){
        switch (key) {
            case GLFW_KEY_ESCAPE:
            {
                glfwSetWindowShouldClose(keyWnd, GLFW_TRUE);
                break;
            }
            case GLFW_KEY_F:
            {
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                break;
            }
            case GLFW_KEY_L:
            {
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                break;
            }
            case GLFW_KEY_I:
            {
                std::cout << "eyePoint: " << to_string( eyePoint ) << '\n';
                std::cout << "verticleAngle: " << fmod(verticalAngle, 6.28f) << ", "
                    << "horizontalAngle: " << fmod(horizontalAngle, 6.28f) << endl;
                break;
            }
            default:
                break;
        }
    }
}

GLuint loadCubemap(vector<string>& faces){
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    int width, height;
    FIBITMAP* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for (GLuint i = 0; i < faces.size(); i++)
    {
        image = FreeImage_Load(FIF_PNG, faces[i].c_str());
        FreeImage_ConvertTo24Bits(image);
        width = FreeImage_GetWidth(image);
        height = FreeImage_GetHeight(image);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height,
            0, GL_RGBA, GL_UNSIGNED_BYTE, image
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

void init_skybox(){
    //shaders
    GLuint vs, fs;
    GLint link_ok;

    vs = create_shader("skybox_vs.glsl", GL_VERTEX_SHADER);
	fs = create_shader("skybox_fs.glsl", GL_FRAGMENT_SHADER);

	program_skybox = glCreateProgram();
	glAttachShader(program_skybox, vs);
	glAttachShader(program_skybox, fs);

	glLinkProgram(program_skybox);
	glGetProgramiv(program_skybox, GL_LINK_STATUS, &link_ok);

	if(link_ok == GL_FALSE){
		std::cout << "Link failed: " << "skybox shaders" << std::endl;
	}

    glUseProgram(program_skybox);

    //buffers
    glGenVertexArrays( 1, &vao_skybox );
    glBindVertexArray( vao_skybox );

    //texture
    vector<string> texture_images;
    texture_images.push_back("right.png");
    texture_images.push_back("left.png");
    texture_images.push_back("bottom.png");
    texture_images.push_back("top.png");
    texture_images.push_back("back.png");
    texture_images.push_back("front.png");

    glActiveTexture(GL_TEXTURE0);
    glGenTextures( 1, &obj_skybox_tex );
    glBindTexture(GL_TEXTURE_CUBE_MAP, obj_skybox_tex);

    for (GLuint i = 0; i < texture_images.size(); i++){
        int width, height;
        FIBITMAP* image;

        image = FreeImage_ConvertTo24Bits( FreeImage_Load(FIF_PNG, texture_images[i].c_str()) );
        //image = FreeImage_ConvertTo24Bits( image );
        width = FreeImage_GetWidth(image);
        height = FreeImage_GetHeight(image);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
            0, GL_RGB, width, height,
            0, GL_BGR, GL_UNSIGNED_BYTE,
            (void*)FreeImage_GetBits(image)
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    uniform_model_skybox = myGetUniformLocation(program_skybox, "model_skybox");
    uniform_view_skybox = myGetUniformLocation(program_skybox, "view_skybox");
    uniform_projection_skybox = myGetUniformLocation(program_skybox, "projection_skybox");

    glUniformMatrix4fv( uniform_model_skybox, 1, GL_FALSE, value_ptr( model_main ) );
    glUniformMatrix4fv( uniform_view_skybox, 1, GL_FALSE, value_ptr( view_main ) );
    glUniformMatrix4fv( uniform_projection_skybox, 1, GL_FALSE, value_ptr( projection_main ) );

    glGenBuffers( 1, &vbo_skybox );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_skybox );
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat)*6*6*3,
        skyboxVertices, GL_STATIC_DRAW
    );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 0 );

    glBindVertexArray(0);
    glUseProgram(0);
}

void init_3d_model(){
    //shaders
    GLuint vs, fs;
    GLint link_ok;

    vs = create_shader("model_vs.glsl", GL_VERTEX_SHADER);
    fs = create_shader("model_fs.glsl", GL_FRAGMENT_SHADER);

    program_model = glCreateProgram();
    glAttachShader(program_model, vs);
    glAttachShader(program_model, fs);

    glLinkProgram(program_model);
    glGetProgramiv(program_model, GL_LINK_STATUS, &link_ok);

    if(link_ok == GL_FALSE){
        std::cout << "Link failed: " << "model shaders" << '\n';
    }

    mesh_info_t mesh = load_obj( "sand_pool.obj" );

    //write vertex coordinate to array
    FACE_INFO& faces = mesh.faceInfos[0];
    faceNumber = faces.size();

    //every face includes 3 vertices, so faceNumber*3
    //every vertex coord includes 3 components, so faceNumber*3*3
    GLfloat* vertex_coords = new GLfloat[ faceNumber*3*3 ];
    GLfloat* texture_coords = new GLfloat[ faceNumber*3*2 ];
    GLfloat* normal_coords = new GLfloat[ faceNumber*3*3 ];

    for (size_t i = 0; i < faceNumber; i++) {
        //vertex coords
        for (size_t j = 0; j < 3; j++) {
            int idxVertex = faces[i].vertexIndices[j];
            vertex_coords[ i*9+j*3+0 ] = mesh.vertexCoords[ idxVertex ].x;
            vertex_coords[ i*9+j*3+1 ] = mesh.vertexCoords[ idxVertex ].y;
            vertex_coords[ i*9+j*3+2 ] = mesh.vertexCoords[ idxVertex ].z;
        }

        //texture coords
        for (size_t j = 0; j < 3; j++) {
            int idxTexture = faces[i].textureCoordIndices[j];
            texture_coords[ i*6+j*2+0 ] = mesh.textureCoords[ idxTexture ].x;
            texture_coords[ i*6+j*2+1 ] = mesh.textureCoords[ idxTexture ].y;
        }

        //normals
        for (size_t j = 0; j < 3; j++) {
            int idxNormal = faces[i].normalIndices[j];
            normal_coords[ i*9+j*3+0 ] = mesh.vertexNormals[ idxNormal ].x;
            normal_coords[ i*9+j*3+1 ] = mesh.vertexNormals[ idxNormal ].y;
            normal_coords[ i*9+j*3+2 ] = mesh.vertexNormals[ idxNormal ].z;
        }
    }

    //buffer objects
    glUseProgram(program_model);
    glGenBuffers( 1, &vbo_model );
    glGenBuffers( 1, &vbo_model_normal );

    glGenVertexArrays( 1, &vao_model );
    glBindVertexArray( vao_model );

    //vbo for pool model
    glBindBuffer( GL_ARRAY_BUFFER, vbo_model );
    glBufferData( GL_ARRAY_BUFFER,
        sizeof( GLfloat ) * faceNumber * 3 * 3,
        vertex_coords, GL_STATIC_DRAW
    );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 0 );

    //vbo for pool texture
    glGenBuffers(1, &obj_pool_tex);
    glBindBuffer(GL_ARRAY_BUFFER, obj_pool_tex);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * faceNumber * 3 * 2,
        texture_coords, GL_STATIC_DRAW
    );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 1 );

    //vertex normal
    glBindBuffer( GL_ARRAY_BUFFER, vbo_model_normal );
    glBufferData( GL_ARRAY_BUFFER,
        sizeof( GLfloat ) * faceNumber * 3 * 3,
        normal_coords, GL_STATIC_DRAW
    );
    glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 2 );

    //texture
    FIBITMAP* poolImage = FreeImage_Load(FIF_PNG, "stone.png");

    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &obj_pool_tex);
    glBindTexture(GL_TEXTURE_2D, obj_pool_tex);

    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB,
        FreeImage_GetWidth(poolImage), FreeImage_GetHeight(poolImage),
        0, GL_BGR, GL_UNSIGNED_BYTE,
        (void*)FreeImage_GetBits(poolImage)
    );
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR
    );

    uniform_tex = myGetUniformLocation(program_model, "tex");
    glUniform1i(uniform_tex, 1);//the 2nd parameter must be same as GL_TEXTUREi

    uniform_model_model = myGetUniformLocation(program_model, "model_model");
    uniform_view_model = myGetUniformLocation(program_model, "view_model");
    uniform_projection_model = myGetUniformLocation(program_model, "projection_model");

    glUniformMatrix4fv( uniform_model_model, 1, GL_FALSE, value_ptr( model_main ) );
    glUniformMatrix4fv( uniform_view_model, 1, GL_FALSE, value_ptr( view_main ) );
    glUniformMatrix4fv( uniform_projection_model, 1, GL_FALSE, value_ptr( projection_main ) );

    //light
    uniform_lightColor = myGetUniformLocation( program_model, "lightColor" );
    glUniform3fv( uniform_lightColor, 1, value_ptr( lightColor ) );

    uniform_lightPosition = myGetUniformLocation( program_model, "lightPosition" );
    glUniform3fv( uniform_lightPosition, 1, value_ptr( lightPosition ) );

    uniform_lightPower = myGetUniformLocation( program_model, "lightPower" );
    glUniform1f( uniform_lightPower, lightPower );

    uniform_diffuseColor = myGetUniformLocation( program_model, "diffuseColor" );
    glUniform3fv( uniform_diffuseColor, 1, value_ptr( materialDiffuseColor ) );

    uniform_ambientColor = myGetUniformLocation( program_model, "ambientColor" );
    glUniform3fv( uniform_ambientColor, 1, value_ptr( materialAmbientColor ) );

    uniform_specularColor = myGetUniformLocation( program_model, "specularColor" );
    glUniform3fv( uniform_specularColor, 1, value_ptr( materialSpecularColor ) );

    glBindVertexArray(0);
    glUseProgram(0);

    delete[] vertex_coords;
    delete[] texture_coords;
    delete[] normal_coords;
}

void init_water(){
    //shaders
    GLuint vs, fs;
    GLint link_ok;

    vs = create_shader("water_vs.glsl", GL_VERTEX_SHADER);
    fs = create_shader("water_fs.glsl", GL_FRAGMENT_SHADER);

    program_water = glCreateProgram();
    glAttachShader(program_water, vs);
    glAttachShader(program_water, fs);

    glLinkProgram(program_water);
	glGetProgramiv(program_water, GL_LINK_STATUS, &link_ok);

	if(link_ok == GL_FALSE){
		std::cout << "Link failed." << std::endl;
	}

    glUseProgram(program_water);
    glGenVertexArrays( 1, &vao_water );
    glBindVertexArray( vao_water );

    glGenBuffers(1, &vbo_water);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_water);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(GLfloat)*6*3,
        waterVertices, GL_STATIC_DRAW
    );
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    uniform_model_water = myGetUniformLocation(program_water, "model_water");
    uniform_view_water = myGetUniformLocation(program_water, "view_water");
    uniform_projection_water = myGetUniformLocation(program_water, "projection_water");

    glUniformMatrix4fv( uniform_model_water, 1, GL_FALSE, value_ptr( model_main ) );
    glUniformMatrix4fv( uniform_view_water, 1, GL_FALSE, value_ptr( view_main ) );
    glUniformMatrix4fv( uniform_projection_water, 1, GL_FALSE, value_ptr( projection_main ) );

    glBindVertexArray(0);
    glUseProgram(0);
}

void init_subscreen1(){
    //shaders
    GLuint vs, fs;
    GLint link_ok;

    vs = create_shader("subscreen1_vs.glsl", GL_VERTEX_SHADER);
    fs = create_shader("subscreen1_fs.glsl", GL_FRAGMENT_SHADER);

    program_subscreen1 = glCreateProgram();
    glAttachShader(program_subscreen1, vs);
    glAttachShader(program_subscreen1, fs);

    glLinkProgram(program_subscreen1);
	glGetProgramiv(program_subscreen1, GL_LINK_STATUS, &link_ok);

	if(link_ok == GL_FALSE){
		std::cout << "Link failed." << std::endl;
	}

    glUseProgram(program_subscreen1);

    //buffers
    glGenVertexArrays(1, &vao_subscreen1);
    glBindVertexArray(vao_subscreen1);

    glGenBuffers( 1, &vbo_subscreen1 );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_subscreen1 );
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(vertices_subscreen1), vertices_subscreen1, GL_STATIC_DRAW
    );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 0 );

    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 0,
        (GLvoid*)(sizeof(GLfloat)*6*3)
    );
    glEnableVertexAttribArray( 1 );

    //
    glGenFramebuffers(1, &fbo_subscreen1);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_subscreen1);

    glActiveTexture(GL_TEXTURE2);
    glGenTextures(1, &obj_subscreen1_tex);
    glBindTexture(GL_TEXTURE_2D, obj_subscreen1_tex);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB,
        WINDOW_WIDTH*2, WINDOW_HEIGHT*2,
        0, GL_RGB, GL_UNSIGNED_BYTE, 0
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenRenderbuffers(1, &subscreen1_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, subscreen1_depth);
    glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
        WINDOW_WIDTH*2, WINDOW_HEIGHT*2
    );
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, subscreen1_depth
    );

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, obj_subscreen1_tex, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);

    GLuint uniform_tex_subscreen1 = myGetUniformLocation(program_subscreen1, "tex_subscreen1");
    glUniform1i(uniform_tex_subscreen1, 2);

    glBindVertexArray(0);
    glUseProgram(0);
}

void init_subscreen2(){
    //shaders
    GLuint vs, fs;
    GLint link_ok;

    vs = create_shader("subscreen2_vs.glsl", GL_VERTEX_SHADER);
    fs = create_shader("subscreen2_fs.glsl", GL_FRAGMENT_SHADER);

    program_subscreen2 = glCreateProgram();
    glAttachShader(program_subscreen2, vs);
    glAttachShader(program_subscreen2, fs);

    glLinkProgram(program_subscreen2);
	glGetProgramiv(program_subscreen2, GL_LINK_STATUS, &link_ok);

	if(link_ok == GL_FALSE){
		std::cout << "Link failed." << std::endl;
	}

    glUseProgram(program_subscreen2);

    //buffers
    glGenVertexArrays(1, &vao_subscreen2);
    glBindVertexArray(vao_subscreen2);

    glGenBuffers( 1, &vbo_subscreen2 );
    glBindBuffer( GL_ARRAY_BUFFER, vbo_subscreen2 );
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(vertices_subscreen2), vertices_subscreen2, GL_STATIC_DRAW
    );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 0 );

    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 0,
        (GLvoid*)(sizeof(GLfloat)*6*3)
    );
    glEnableVertexAttribArray( 1 );

    //
    glGenFramebuffers(1, &fbo_subscreen2);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_subscreen2);

    glActiveTexture(GL_TEXTURE3);
    glGenTextures(1, &obj_subscreen2_tex);
    glBindTexture(GL_TEXTURE_2D, obj_subscreen2_tex);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB,
        WINDOW_WIDTH*2, WINDOW_HEIGHT*2,
        0, GL_RGB, GL_UNSIGNED_BYTE, 0
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glGenRenderbuffers(1, &subscreen2_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, subscreen2_depth);
    glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
        WINDOW_WIDTH*2, WINDOW_HEIGHT*2
    );
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, subscreen2_depth
    );

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, obj_subscreen2_tex, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT2);

    GLuint uniform_tex_subscreen2 = myGetUniformLocation(program_subscreen2, "tex_subscreen2");
    glUniform1i(uniform_tex_subscreen2, 3);

    glBindVertexArray(0);
    glUseProgram(0);
}

void drawSkybox(mat4& M, mat4& V, mat4& P){
    glUseProgram(program_skybox);
    glBindVertexArray(vao_skybox);
    glUniformMatrix4fv( uniform_model_skybox, 1, GL_FALSE, value_ptr( M ) );
    glUniformMatrix4fv( uniform_view_skybox, 1, GL_FALSE, value_ptr( V ) );
    glUniformMatrix4fv( uniform_projection_skybox, 1, GL_FALSE, value_ptr( P ) );
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawModels(mat4& M, mat4& V, mat4& P){
    glUseProgram(program_model);
    glBindVertexArray(vao_model);
    glUniformMatrix4fv( uniform_view_model, 1, GL_FALSE, value_ptr( V ) );
    glUniformMatrix4fv( uniform_projection_model, 1, GL_FALSE, value_ptr( P ) );
    glDrawArrays(GL_TRIANGLES, 0, faceNumber*3);
}

void drawWater(mat4& M, mat4& V, mat4& P){
    glUseProgram(program_water);
    glBindVertexArray(vao_water);
    glUniformMatrix4fv( uniform_view_water, 1, GL_FALSE, value_ptr( V ) );
    glUniformMatrix4fv( uniform_projection_water, 1, GL_FALSE, value_ptr( P ) );
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawSubscreen1(){
    glUseProgram(program_subscreen1);
    glBindVertexArray(vao_subscreen1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawSubscreen2(){
    glUseProgram(program_subscreen2);
    glBindVertexArray(vao_subscreen2);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
