#ifdef _WIN32
#include <conio.h>
#endif

#include <thread>
#include <atomic>

//math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/random.hpp>

//graphics
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <Wretched/System/Settings.h>
#include <Wretched/System/Entity.h>
#include <Wretched/System/Thread.h>
#include <Wretched/Math/Math.h>
#include <Wretched/Core/Placement.h>
#include <Wretched/Core/Audio.h>
#include <Wretched/Graphics/Rendering.h>
#include <Wretched/Graphics/Font.h>
#include <Wretched/Graphics/Framerate.h>
#include <Wretched/Graphics/Interface.h>
#include <Wretched/Graphics/Shader.h>
#include <Wretched/Graphics/Camera.h>
#include <Wretched/Graphics/Structures.h>
#include <Wretched/Graphics/GLError.h>
#include <Wretched/Graphics/Lighting.h>

#include "PlayerScript.cpp"

#ifdef _WIN32
#define WINDIR(x) (std::string("C:") + x).c_str()
#elif __linux__
#define WINDIR(x) (std::string("/run/media/lavorgia/Personal Files") + x).c_str()
#define ASSETDIR(x) (std::string("/mnt/Windows/Users/AllFather/ClionProjects/Schism/Assets/") + x).c_str()
#endif
std::atomic<bool> exitclient, mainexit;
using namespace Wretched;
#define WRETCHED_ENABLE_DEFERRED
#define WRETCHED_ENABLE_LIGHTING
//#define WRETCHED_ENABLE_TEXT
//#define WRETCHED_ENABLE_GUI
Entity::Manager entmgr;
Audio::Manager audio(entmgr);
//Placement::System plamgr;
Entity *flylo = entmgr.createentity();
Audio::Sound *fly = flylo->createcomponent<Audio::Sound>(audio, ASSETDIR("Sound/N.E.R.V. after hours - Drip Drop - (LEFT).wav"));
FramerateManager fps(60.0025f);

void rendering_loop();

namespace Wretched {
  static struct sys {
    sys() {
      std::printf("begin:\n");
      srand(time(NULL));
#ifdef _WIN32
      SetConsoleTitle("Debug");
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0B);// 0x0B|0x20|0x40|BACKGROUND_INTENSITY);//FOREGROUND_BLUE
      system("mode CON: LINES=300");
      //system("cls");
      //MoveWindow(GetConsoleWindow(), 900, 0, 800, 300, TRUE);gth
#endif
      audio.setrelative(*fly, false);
      audio.loop(*fly, true);

      //alSourcef(audio[flylo].get()->source, AL_CONE_OUTER_ANGLE,50);
      Thread rendering(rendering_loop);
      rendering.join();
    }
  } sys;
}

void rendering_loop() {
  std::cout << "started it" << std::endl;
  std::cout << "okay === " << std::endl;
  std::string basetitle("Wretched Engine");
  sf::ContextSettings winsettings(24, 0, 0, 4, 5);
  sf::Window win;
  try {
    win.create(sf::VideoMode(1, 1), basetitle.c_str(), sf::Style::Resize | sf::Style::Close, winsettings);
  } catch (std::exception &e) {
    std::printf("window could not load\n");
    return;
  }
  win.setKeyRepeatEnabled(false);
  win.setPosition(sf::Vector2i(0, 0));
  win.setSize(sf::Vector2u(800, 450));
  win.setVisible(false);
  glewExperimental = true;
  if (glewInit() != GLEW_OK) {
    std::cout << "glew could not properly initialize. Press Enter to Exit" << std::endl;
    std::cin.get();
    return;
  };
  int numbits;
  glGetIntegerv(GL_DEPTH_BITS, &numbits);
  std::cout << numbits << std::endl;
  int nume;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GLuint dtex;
  glGenTextures(1, &dtex);
  glBindTexture(GL_TEXTURE_2D, dtex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 1, 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dtex, 0);
  //glGetNamedFramebufferAttachmentParameteriv(0, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &nume);
  //glGetNamedFramebufferAttachmentParameteriv(0, GL_FRONT, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &nume);
  //glEnable(GL_CULL_FACE);
  //std::cout << nume << std::endl;
  glCullFace(GL_BACK);
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glDepthMask(GL_TRUE);
  glDepthRange(-16000, 16000);
  GLubyte *vers = (GLubyte *) glGetString(GL_VERSION);
  std::cout << "initialized" << " GL version:" << vers << std::endl;
  std::string gc((char *) glGetString(GL_RENDERER));
  std::string GC = std::string((char *) glGetString(GL_VENDOR)) + "\n";
  GC += gc.substr(0, gc.find_first_of("//") - 1) + "\n";
  GC += std::string((char *) glGetString(GL_VERSION)) + "\n";//GL_SHADING_LANGUAGE_VERSION
  GC += std::string((char *) glGetString(GL_SHADING_LANGUAGE_VERSION)) + "\n";
  printf("%s", GC.c_str());
  std::cout << "max size comp:" << GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS << "\n";
#ifdef WRETCHED_ENABLE_DEFERRED
#ifdef WRETCHED_ENABLE_TEXT
  ShaderProgram TextS(WINDIR("/Users/AllFather/Downloads/Libraries/include/Wretched/Assets/Graphics/TextShader.vert")), WINDIR("/Users/AllFather/Downloads/Libraries/include/Wretched/Assets/Graphics/TextShader.frag"));
FontLibrary fontengine;
fontengine.setshader(TextS.PID);
Font Forum(fontengine, "Arial.ttf");//Forum-Regular
Forum.color = glm::vec4(0, 0, 1, 1);
#endif WRETCHED_ENABLE_TEXT
  ComputeProgram CompS(
     ASSETDIR("/Graphics/ComputeShader.comp"));
  Graphics::GL::Texture2D Composite;
  Composite.bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


  Graphics::GL::VertexArray ScreenVA;
  Graphics::GL::BufferObject ScreenVBO(GL_ARRAY_BUFFER);
  ScreenVA.bind();
  glVertexAttribFormat(0, 4, GL_FLOAT, GL_TRUE, 0);
  glVertexAttribBinding(0, 0);
  glEnableVertexAttribArray(0);
  ScreenVBO.bind();
  GLfloat square[4][4] = {
      {0, 0, 0, 0},
      {1, 0, 1, 0},
      {1, 1, 1, 1},
      {0, 1, 0, 1}
  };
  ScreenVBO.data(sizeof(square), square, GL_STATIC_DRAW);
  glBindVertexBuffer(0, ScreenVBO._name, 0, 4 * sizeof(float));
  ScreenVA.release();
#endif
  ShaderProgram
      shade(
        ASSETDIR("/Graphics/DeferredShader1.vert"),
        ASSETDIR("/Graphics/DeferredShader1.frag")),
      ScreenSpace(
        ASSETDIR("/Graphics/ScreenSpace.vert"),
        ASSETDIR("/Graphics/ScreenSpace.frag")),
      GUIshade(
        ASSETDIR("/Graphics/GUIShader.vert"),
        ASSETDIR("/Graphics/GUIShader.frag"));
  shade.use();
  //GLCheckError();
  Graphics::System ded(entmgr);
  std::vector<Graphics::Object *> Atlas, Kasumin;
  std::string loadstr(ASSETDIR("Models/FA-22_Raptor/FA-22_Raptor.obj"));//Kasumi_Ninja_Outfit/Kasumi_Ninja_Outfit.obj//3d temp/FA-22_Raptor/FA-22_Raptor.obj

  int t = 3, c = 4;
  for (int i = 0; i < t; ++i) {
    Entity *meteor = entmgr.createentity();

    Graphics::Object *n = meteor->createcomponent<Graphics::Object>(ded, ASSETDIR("Models/meteor1/meteor1.obj"));
    ////Atlas_Titan/Atlas_Titan.obj/SkySphere/SkySphere.obj
    auto a = meteor->createcomponent<Placement::Placement>();
    a->position = GridPosition(glm::vec3(2, 0, 0), glm::vec3((float) (i - (t - 1) / 2) * 2000 + 15, 0, 600));
    //3||18,0,5 for atlass////FA-22_Raptor/FA-22_Raptor.obj
    n->scale = 500;
    Atlas.push_back(n);
  }

  Entity *player = entmgr.createentity();
  Graphics::Object *playerobj(player->createcomponent<Graphics::Object>(ded, loadstr.c_str()));
  playerobj->scale = 1;
  Placement::Placement *playerplace = player->createcomponent<Placement::Placement>();
  player->createcomponent<PlayerScript>(player);//edit

  for (int i = 0; i < sqrt(c); ++i) {
    for (int g = 0; g < sqrt(c); ++g) {
      std::cout << "loading : " << loadstr.c_str() << std::endl;
      Entity *nn = entmgr.createentity();
      Graphics::Object *n = nn->createcomponent<Graphics::Object>(ded, loadstr.c_str());
      auto a = nn->createcomponent<Placement::Placement>();
      n->scale = 1;
      a->position = GridPosition({2, 0, 0},
                                 glm::vec3((i - (sqrt(c) - 1) / 2) * 15, 0, (float) -g * 20));//was 3,0,1.5 for kasumi/
      Kasumin.push_back(n);
    }
  }

  Entity *test = entmgr.createentity();
  Graphics::Object *uuu = test->createcomponent<Graphics::Object>(ded, loadstr.c_str());
  auto *ggg = test->createcomponent<Placement::Placement>();
  ggg->position = GridPosition({0, 0, 0}, glm::vec3(15, 15, 15));
  Entity *cawa = entmgr.createentity();
  Graphics::Object *cawado = cawa->createcomponent<Graphics::Object>(ded,ASSETDIR("Models/s8m87k8iopa8-space_station/space_station.obj"));
  cawado->scale = 10000;
  auto cawapos = cawa->createcomponent<Placement::Placement>();
  cawapos->position = GridPosition({2, 0, 0}, {0, -8000, 0});
  std::string bgscreen(ASSETDIR("Models/SkySphere/SkySphere.obj"));
  Entity *bground = entmgr.createentity();
  Graphics::Object *background = bground->createcomponent<Graphics::Object>(ded, bgscreen.c_str());//ded.createcomponent(bground);
  auto bgpos = bground->createcomponent<Placement::Placement>();
  background->nodepthtest = true;
  //GLCheckError();
#ifdef WRETCHED_ENABLE_DEFERRED
  Graphics::GL::GraphicsBuffer gray(GL_RGBA32F, GL_RGBA, 6, GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT);//was 5//then 3
#endif
#ifdef WRETCHED_ENABLE_LIGHTING
  std::vector<Entity *> llist;
  Lighting::Manager lightbox;
  lightbox.bind(0);


  Entity *sunlight = entmgr.createentity(), *light1 = entmgr.createentity(), *light2 = entmgr.createentity();

  auto slight = sunlight->createcomponent<Lighting::Light>();
  auto sunplace = sunlight->createcomponent<Placement::Placement>();
  slight->color = glm::vec3(.7f, .7f, .5f);
  slight->radius = 1400000;
  sunplace->position = GridPosition({2, 1, 0}, {0, 0, 0});

  auto l1light = light1->createcomponent<Lighting::Light>();
  l1light->color = glm::vec3(0, 1, 0);
  auto l1pos = light1->createcomponent<Placement::Placement>();
  l1pos->position = GridPosition({2, 1, 0}, {0, 0, -100});

  auto l2light = light2->createcomponent<Lighting::Light>();
  l2light->color = glm::vec3(1, 0, 0);
  auto l2pos = light2->createcomponent<Placement::Placement>();
  l2pos->position = GridPosition({2, 1, 0}, {0, 0, 0});
  int l = 3;///3 c
  for (int i = 0; i < l; ++i) {
    for (int g = 0; g < l; ++g) {
      auto *templ = entmgr.createentity();
      auto L = templ->createcomponent<Lighting::Light>();
      auto Lpos = templ->createcomponent<Placement::Placement>();
      L->radius = 75;
      L->color = glm::vec3(float(rand()) / (RAND_MAX), 0., float(rand()) / (RAND_MAX)) * .25f;
      Lpos->position = GridPosition({2, 0, 0}, glm::vec3(5000, 1, 0) +
                                               glm::vec3(4000.f * (i + 1), 0, -4000.f * (g - (l - 1) / 2.f)));
      llist.push_back(templ);
    }
  }
#endif
  std::cout << "start" << std::endl;
#ifdef WRETCHED_ENABLE_GUI
  Graphics::GL4Interface masterbox;
masterbox.LoadXML(WINDIR("/Users/AllFather/Downloads/Libraries/include/Wretched/Assets/GUI/Main.xml", entmgr));
#endif WRETCHED_ENABLE_GUI
  bool fpers = false;
  bool resswitch = false;
  float rottimer = 0;
  float speed = 0;
  GridPosition prevpos;
  Entity *ecam = entmgr.createentity();
  Camera *cam = ecam->createcomponent<Camera>();
  Placement::Placement *camplace = ecam->createcomponent<Placement::Placement>();
  camplace->position = playerplace->position + 20.f * camplace->frontdir();
  playerplace->position = GridPosition({2, 1, 0}, {0.f, 10.f, 25.f});
  glm::vec4 backgroundcolor(.5f, .5f, .5f, 1.0f);

  sf::Event Event;
  bool WindowHasFocus(false), inwindow(true), interacting(false), Deferred(false), WIREFRAME(false);
  float timecounter(0);
  glm::ivec2 LMBlast(0);
  bool lheld = false;
  bool lastout = false;
  glClearColor(.0f, .0f, .03f, 1.f);
  float stfps = 0, lrt = 0;
  int cc(0), b(0);
  win.setVisible(true);
  bool pilotmode = true;
  bool nolights = false;

  float tilesize = 32.f;
  glm::quat s0, s1, s2;
  auto playerobject = player->getcomponent<PlayerScript>();
  playerobject->init();
  while (win.isOpen()) {
    if (!fps.govern()) {
      std::this_thread::sleep_for(
          std::chrono::microseconds((int) ((1. / fps.getidealfps() - 1. / fps.getfps()) * 1000000.)));
      continue;
    }
    int width(win.getSize().x), height(win.getSize().y);
    auto colddelta = (float) fps.deltatime();
    lastout = inwindow;
    if (win.pollEvent(Event)) {
      if (Event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        win.close();
        break;
      }
      if (Event.type == sf::Event::Resized) {
        glViewport(0, 0, Event.size.width, Event.size.height);
#ifdef WRETCHED_ENABLE_TEXT
        fontengine.setviewport(Event.size.width, Event.size.height);
#endif
#ifdef WRETCHED_ENABLE_GUI
        masterbox.setviewport(Event.size.width, Event.size.height);
#endif
      }
      if (Event.type == sf::Event::LostFocus) WindowHasFocus = false;
      if (Event.type == sf::Event::MouseEntered) inwindow = true;
      if (Event.type == sf::Event::MouseLeft) inwindow = false;
      if (Event.type == sf::Event::GainedFocus) WindowHasFocus = true;
    }
    glm::ivec2 mousepos(sf::Mouse::getPosition(win).x, sf::Mouse::getPosition(win).y);
#ifdef WRETCHED_ENABLE_GUI
    masterbox.updateinput(mousepos.x, height - mousepos.y, sf::Mouse::isButtonPressed(sf::Mouse::Left),
                     orrent linux sf::Mouse::isButtonPressed(sf::Mouse::Right), Event.key.code,
                      sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl),
                      sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift),
                      sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt),
                      sf::Keyboard::isKeyPressed(sf::Keyboard::LSystem) || sf::Keyboard::isKeyPressed(sf::Keyboard::RSystem),
                      Graphics::Interface::Mouse | Graphics::Interface::Keyboard);
masterbox.update();
#endif
    auto a = true;
    if (win.hasFocus()) {
      auto space = sf::Keyboard::isKeyPressed(sf::Keyboard::Space), shift = sf::Keyboard::isKeyPressed(
          sf::Keyboard::LShift);
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        playerplace->position.move(-camplace->rightdir() * colddelta * 5.f * playerobj->scale);
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        playerplace->position.move(camplace->rightdir() * colddelta * 5.f * playerobj->scale);
      if (space && shift) {
        speed -= colddelta * 30.f;
      }
      else if (space) {
        speed += colddelta * 40.f;
      }
      else {
        speed = speed > 0 ? glm::max(0.f, speed - colddelta * 10.f) : glm::min(0.f, speed + colddelta * 10.f);
      }
      playerobject->update(colddelta);
//      if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)) {
//        Placement::System::pitch(*playerplace, (float) colddelta * W_degree * 100.f);
//      }
//      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
//        Placement::System::pitch(*playerplace, (float) colddelta * W_degree * -100.f);
//      }
//      if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
//        Placement::System::roll(*playerplace, (float) colddelta * W_degree *
//                                               200.f);//playerobj->lroll(200.f * colddelta * W_degree); //plamgr.roll(*player, 200.f * (float)colddelta * W_degree);
//      if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
//        Placement::System::roll(*playerplace, (float) colddelta * W_degree *
//                                               -200.f); //plamgr.roll(*player, -200.f * (float)colddelta * W_degree);
//      if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
//        Placement::System::yaw(*playerplace, (float) colddelta * W_degree * 200.f);
//      if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
//        Placement::System::yaw(*playerplace, (float) colddelta * W_degree * -200.f);
//      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)) {
//        cam->zoom -= (float) (colddelta * (cam->zoom + 1 / W_pi) / 4);
//      }
//      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)) {
//        cam->zoom += (float) (colddelta * (121 / W_pi - cam->zoom) / 4);
//      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) { }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) { }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) { }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket)) {
        double i;
        float p;
        p = modf(float(audio.getplaybackpossec(*fly) - 11.f * colddelta) / audio.getduration(*fly), &i);;
        audio.seek(*fly, p > 0 ? p : 1.f + p);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket)) {
        double i;
        float p;
        p = modf(float(audio.getplaybackpossec(*fly) + 9.f * colddelta) / audio.getduration(*fly), &i);
        audio.seek(*fly, p > 0 ? p : 1.f + p);
      }
      if (Event.type == sf::Event::KeyPressed) {

        if ((Event.key.code == sf::Keyboard::F)) {
          fpers = !fpers;
        }
        if (Event.key.code == sf::Keyboard::BackSlash) {
          if (audio.isplaying(*fly)) {
            audio.pause(*fly);
          }
          else audio.play(*fly);
        }
        if ((Event.key.code == sf::Keyboard::PageUp)) {
          audio.stop(*fly);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::End)) { }
        if ((Event.key.code == sf::Keyboard::L)) {
          nolights = !nolights;
        }
        if ((Event.key.code == sf::Keyboard::R)) {
          resswitch = !resswitch;
          if (resswitch) {
            win.setSize(sf::Vector2u(800, 600));
          }
          else {
            win.setSize(sf::Vector2u(800, 450));
          }
        }
        if ((Event.key.code == sf::Keyboard::P)) {
          pilotmode = !pilotmode;
        }
        if ((Event.key.code == sf::Keyboard::D)) {
          Deferred = !Deferred;
          Deferred ? glClearColor(.1f, .1f, .1f, 1.f) : glClearColor(.0f, .0f, .03f, 1.f);
        }
        if ((Event.key.code == sf::Keyboard::W)) {
          (WIREFRAME = !WIREFRAME) ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK,
                                                                                               GL_LINE);
        }
      }
#ifdef WRETCHED_ENABLE_GUI
#define guidebugcheck() masterbox.ownsinput()
#else
#define guidebugcheck() false
#endif
      if (!guidebugcheck() && inwindow) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {// &&
          a = false;
          win.setMouseCursorVisible(false);
          if (!lheld) {
            lheld = true;
            LMBlast = mousepos;
          }
          else {
            glm::ivec2 mousechange = LMBlast - mousepos;
            camplace->pitch(.1f * mousechange.y * colddelta);
            camplace->yaw(.1f * mousechange.x * colddelta);
          }
          sf::Mouse::setPosition(sf::Vector2i(LMBlast.x,LMBlast.y), win);
        }
        else if (lheld) {
          lheld = false;
          sf::Mouse::setPosition(sf::Vector2i((int) LMBlast[0], (int) LMBlast[1]), win);
          win.setMouseCursorVisible(true);
        }
      }
    }
    {
      timecounter += colddelta;//keep all objects updating uniformly
      if (timecounter > 2) {
        timecounter = 0;
        win.setTitle((basetitle + "(" + std::to_string(stfps) + ") speed:" + std::to_string(speed)).c_str());
        if (prevpos != playerplace->position) {
          std::cout << "user 1 pos \n" << playerplace->position << std::endl;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket)) {
          printf("p: %f , dur: %f\n", audio.getplaybackpossec(*fly), audio.getduration(*fly));
          printf("audio file: %s\n", fpath::getfilename(fly->filename).c_str());
        }
        prevpos = playerplace->position;
      }
      cawapos->roll(float(.1f * colddelta));
      playerplace->position.move(-playerplace->frontdir() * speed);
      if (fpers) {
        camplace->position = playerplace->position - playerplace->frontdir() * 5.55f - playerplace->updir() * .575f;
        if (a && pilotmode)
          camplace->orientation = glm::slerp(camplace->orientation, playerplace->orientation,
                                             glm::min((float) colddelta * 9.f, 1.f));
      } else {
        camplace->position = playerplace->position + camplace->frontdir() * 20.f * playerobj->scale;
        if (a && pilotmode)
          camplace->orientation = glm::slerp(camplace->orientation, playerplace->orientation,
                                             glm::min((float) colddelta * 3.f, 1.f));
      }
      audio.setposition(glm::vec3(0));

      //audio.setvelocity(playerobj->frontdir()*speed);
      audio.setorientation(camplace->frontdir(), camplace->updir());
      audio.setposition(*fly, (camplace->position - GridPosition(glm::vec3(2, 0, 0), glm::vec3(0, -8000, 0))).getabsposition());
      cam->update(width, height);
      bgpos->position = camplace->position;
      background->scale = 160000;
      stfps = (float) fps.getavgfps();
#ifdef WRETCHED_ENABLE_DEFERRED
      if (Deferred) {
        gray.begindraw(width, height); //set viewport and draw to framebuffer
      }
#endif
      {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shade.use();
        ded.setcursor(mousepos);
        cam->setcutoff(.1f, 100000.f);
        glm::mat4 camco = cam->proj() * cam->view(*camplace);
        glUniformMatrix4fv(0, 1, GL_FALSE, (float *) &camco[0][0]);
        ded.curr_cam = ecam;
        ded.render();
      }

#ifdef WRETCHED_ENABLE_DEFERRED
      if (Deferred) {///finish geometry
#ifdef WRETCHED_ENABLE_LIGHTING
        lightbox.clear();
        lightbox.render(*ecam, *sunlight);
        lightbox.render(*ecam, *light1);
        lightbox.render(*ecam, *light2);
        rottimer = (float) std::chrono::high_resolution_clock::now().time_since_epoch().count();
        rottimer *= W_pi / (50.f);
        for (auto each = 0; each < llist.size(); ++each) {
          auto ent = llist[each];
          auto light = ent->getcomponent<Lighting::Light>();
          auto npos = ent->getcomponent<Placement::Placement>();
          GridPosition ogpos = npos->position;
          npos->position += glm::vec3(sin(rottimer + each), 0, cos(rottimer + each)) *
                            100.f;//glm::vec3(10.f *(rand() / (RAND_MAX + 1) - .5f), 0, 10.f * (rand() / (RAND_MAX + 1) - .5f));
          light->radius = 15000;
          light->color.g = float(each) / (llist.size() - 1);
          lightbox.render(*ecam, *ent);
          npos->position = ogpos;
        }
        lightbox.load();
#endif
        gray.read();///prep for read from non-default drawbuffer
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);///prep for write to default drawbuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);///clear default drawbuffer

        glActiveTexture(GL_TEXTURE0 + gray.frame_textures.size() + 1);
        Composite.bind();
        Composite.store(0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glBindImageTexture(0, Composite.getID(), 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA16F);//GL_READ_WRITE
        CompS.use();
        glm::mat4 vp = cam->proj() * cam->view(*camplace);
        glm::mat4 inv(glm::inverse(vp));
        glUniformMatrix4fv(0, 1, GL_FALSE, &cam->proj()[0][0]);
        glUniformMatrix4fv(1, 1, GL_FALSE, &cam->view(*camplace)[0][0]);
        glUniformMatrix4fv(2, 1, GL_FALSE, &vp[0][0]);
        glUniformMatrix4fv(3, 1, GL_FALSE, &inv[0][0]);

        CompS.dispatch((const unsigned int) ceil(float(width) / tilesize),
                       (const unsigned int) ceil(float(height) / tilesize), 1);
        glActiveTexture(GL_TEXTURE0);
        Composite.bind();
        ScreenSpace.use();
        ScreenVA.bind();
        glDrawArrays(GL_QUADS, 0, 4);
        ScreenVA.release();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
          gray.displaychannel(Graphics::GL::GraphicsBuffer::DIFFUSE, (int) 0, (int) 0, .25);
          gray.displaychannel(1, (int) 0, (int) (height * .25), .25);
          gray.displaychannel(2, (int) 0, (int) (height * .5), .25);
          gray.displaychannel(3, (int) (width * .75), (int) 0, .25);
          gray.displaychannel(4, (int) (width * .75), (int) (height * .25), .25);
          gray.displaychannel(5, (int) (width * .75), (int) (height * .5), .25);
          //gray.displaydepthchannel((int)0, (int)height*.75, .25);
          ///draw select channels from buffer
        }
        ///clean up
      }

      glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
      glActiveTexture(GL_TEXTURE0);
#endif
#ifdef WRETCHED_ENABLE_TEXT
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
  Forum.color = glm::vec4(0, 0, 1, 1);
  Forum.render(.1, .8, .05, false, "Wretched Engine,\nfps: %.2f", stfps);
};
#endif
#ifdef WRETCHED_ENABLE_GUI
      GUIshade.use();
ded.setcursor(mousepos);
masterbox.render();
ded.updatehover();
#endif

      //glGetNamedFramebufferAttachmentParameteriv(gray.frame_buffer_object, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &numbits);
      //glGetNamedFramebufferAttachmentParameteriv(0, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &numbits);
      //glGetNamedFramebufferAttachmentParameteriv(0, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &numbits);
      win.display();
    }
  }
}

/*
update input
update logic
update physics
render all Meshes
render all GUIComponents
*/
int main() {
  return 0;
}
