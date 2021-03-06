#if 0
rm -f a.out
cc -std=c99 main.c ../rei/rei.c ../glfw/lib/libglfw.so.3 -lGL -lm
LD_LIBRARY_PATH=../glfw/lib/ ./a.out
exit
#endif

#include "../rei/rei.h"
#define GLFW_INCLUDE_NONE
#include "../glfw/include/glfw3.h"

#include <stdio.h>
#include <math.h>

const float mesh_vertices[] = {
#include "mesh_vertices.h"
};

#define countof(x) (sizeof(x) / sizeof((x)[0]))

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_RESIZABLE, 0);
  glfwWindowHint(GLFW_SAMPLES, 4);
  GLFWwindow * window = glfwCreateWindow(700, 700, "", NULL, NULL);
  glfwMakeContextCurrent(window);

  ReiContext   context = {};
  ReiContext * ctx     = &context;
  reiCreateContext((ReiTypeProcedureGetProcAddress)glfwGetProcAddress, &context);

  char vp_string[] =
    "!!ARBvp1.0"
    "TEMP pos;"
    "TEMP R0;"
    "ADD pos, vertex.position, -program.local[0];"
    "MUL R0.xyzw, pos.xzzx, program.local[1].xyxy;"
    "ADD pos.x, R0.x,-R0.y;"
    "ADD pos.z, R0.z, R0.w;"
    "MUL R0.xyzw, pos.yzzy, program.local[1].zwzw;"
    "ADD pos.y, R0.x,-R0.y;"
    "ADD pos.z, R0.z, R0.w;"
    "MOV result.color, vertex.color;"
    "MOV result.position.xyw, pos.xyzz;"
    "MOV result.position.z, 0.1;"
    "END";

  char fp_string[] =
    "!!ARBfp1.0"
    "MOV result.color, fragment.color;"
    "END";

  ReiHandleProgram vp_program = reiCreateProgram(ctx);
  ReiHandleProgram fp_program = reiCreateProgram(ctx);

  reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, vp_program);
  reiProgramInitialize(ctx, REI_PROGRAM_BINDING_VERTEX, sizeof(vp_string)-1, vp_string);
  reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, 0);

  reiBindProgram(ctx, REI_PROGRAM_BINDING_FRAGMENT, fp_program);
  reiProgramInitialize(ctx, REI_PROGRAM_BINDING_FRAGMENT, sizeof(fp_string)-1, fp_string);
  reiBindProgram(ctx, REI_PROGRAM_BINDING_FRAGMENT, 0);

  ReiHandleCommandList list = reiCreateCommandList(ctx);
  reiCommandListSet(ctx, list);
    reiCommandSetViewport(ctx, 0, 0, 700, 700);
    reiCommandSetScissor(ctx, 0, 0, 700, 700);
    reiCommandClear(ctx, REI_CLEAR_DEPTH_BIT | REI_CLEAR_COLOR_BIT, 0.f, 0, 0.f, 0.f, 0.05f, 1.f);
    ReiMeshState mesh_state = {};
    mesh_state.programVertex                                  = vp_program;
    mesh_state.programFragment                                = fp_program;
    mesh_state.rasterizationDepthClampEnable                  = 0;
    mesh_state.rasterizationCullMode                          = REI_CULL_MODE_NONE;
    mesh_state.rasterizationFrontFace                         = REI_FRONT_FACE_COUNTER_CLOCKWISE;
    mesh_state.rasterizationDepthBiasEnable                   = 0;
    mesh_state.rasterizationDepthBiasConstantFactor           = 0;
    mesh_state.rasterizationDepthBiasSlopeFactor              = 0;
    mesh_state.multisampleEnable                              = 1;
    mesh_state.multisampleAlphaToCoverageEnable               = 0;
    mesh_state.multisampleAlphaToOneEnable                    = 0;
    mesh_state.depthTestEnable                                = 1;
    mesh_state.depthTestDepthWriteEnable                      = 1;
    mesh_state.depthTestDepthCompareOp                        = REI_COMPARE_OP_GREATER_OR_EQUAL;
    mesh_state.stencilTestEnable                              = 0;
    mesh_state.stencilTestFrontStencilTestFailOp              = REI_STENCIL_OP_KEEP;
    mesh_state.stencilTestFrontStencilTestPassDepthTestPassOp = REI_STENCIL_OP_KEEP;
    mesh_state.stencilTestFrontStencilTestPassDepthTestFailOp = REI_STENCIL_OP_KEEP;
    mesh_state.stencilTestFrontCompareOp                      = REI_COMPARE_OP_NEVER;
    mesh_state.stencilTestBackStencilTestFailOp               = REI_STENCIL_OP_KEEP;
    mesh_state.stencilTestBackStencilTestPassDepthTestPassOp  = REI_STENCIL_OP_KEEP;
    mesh_state.stencilTestBackStencilTestPassDepthTestFailOp  = REI_STENCIL_OP_KEEP;
    mesh_state.stencilTestBackCompareOp                       = REI_COMPARE_OP_NEVER;
    mesh_state.stencilTestFrontAndBackCompareMask             = 0;
    mesh_state.stencilTestFrontAndBackWriteMask               = 0;
    mesh_state.stencilTestFrontAndBackReference               = 0;
    mesh_state.blendLogicOpEnable                             = 0;
    mesh_state.blendLogicOp                                   = REI_LOGIC_OP_CLEAR;
    mesh_state.blendConstants[0]                              = 0;
    mesh_state.blendConstants[1]                              = 0;
    mesh_state.blendConstants[2]                              = 0;
    mesh_state.blendConstants[3]                              = 0;
    mesh_state.outputColorWriteEnableR                        = 1;
    mesh_state.outputColorWriteEnableG                        = 1;
    mesh_state.outputColorWriteEnableB                        = 1;
    mesh_state.outputColorWriteEnableA                        = 1;
    mesh_state.outputColorBlendEnable                         = 0;
    mesh_state.outputColorBlendColorFactorSource              = REI_BLEND_FACTOR_ZERO;
    mesh_state.outputColorBlendColorFactorTarget              = REI_BLEND_FACTOR_ZERO;
    mesh_state.outputColorBlendColorOp                        = REI_BLEND_OP_ADD;
    mesh_state.outputColorBlendAlphaFactorSource              = REI_BLEND_FACTOR_ZERO;
    mesh_state.outputColorBlendAlphaFactorTarget              = REI_BLEND_FACTOR_ZERO;
    mesh_state.outputColorBlendAlphaOp                        = REI_BLEND_OP_ADD;
    reiCommandMeshSetState(ctx, &mesh_state);
    reiCommandMeshSet(ctx);
    for (int i = 0, mesh_vertices_count = countof(mesh_vertices) / 3; i < mesh_vertices_count; i += 1) {
      reiCommandMeshColor(ctx, i * 0.00025f, 0, 0.1f, 1);
      reiCommandMeshPosition(ctx, mesh_vertices[i * 3 + 0],
                                  mesh_vertices[i * 3 + 1],
                                  mesh_vertices[i * 3 + 2], 1);
    }
    reiCommandMeshEnd(ctx);
  reiCommandListEnd(ctx);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  float pos_x = 0;
  float pos_y = 0;
  float pos_z = -2.f;
  float rot_x = 0;
  float rot_y = 0;

  glfwPollEvents();

  double start_mouse_x = 0;
  double start_mouse_y = 0;
  glfwGetCursorPos(window, &start_mouse_x, &start_mouse_y);
  double mouse_x_prev = start_mouse_x;
  double mouse_y_prev = start_mouse_y;

  for (; glfwWindowShouldClose(window) == 0;) {
    glfwPollEvents();

    double mouse_x = 0;
    double mouse_y = 0;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);

    float mouse_disp_x = (float)(mouse_x_prev - mouse_x) * 0.0035f;
    float mouse_disp_y = (float)(mouse_y - mouse_y_prev) * 0.0035f;

    rot_x += mouse_disp_x;
    rot_y += mouse_disp_y;

    float move_vec_x = 0;
    float move_vec_y = 0;
    float move_vec_z = 0;

    move_vec_x += glfwGetKey(window, GLFW_KEY_D);
    move_vec_x -= glfwGetKey(window, GLFW_KEY_A);

    move_vec_y += glfwGetKey(window, GLFW_KEY_E);
    move_vec_y -= glfwGetKey(window, GLFW_KEY_Q);

    move_vec_z += glfwGetKey(window, GLFW_KEY_W);
    move_vec_z -= glfwGetKey(window, GLFW_KEY_S);

    float move_vec_len = sqrtf(move_vec_x*move_vec_x + move_vec_y*move_vec_y + move_vec_z*move_vec_z);
    if (move_vec_len != 0) {
      move_vec_x /= move_vec_len;
      move_vec_y /= move_vec_len;
      move_vec_z /= move_vec_len;
    }

    {
      float rot_cos = cosf(rot_y);
      float rot_sin = sinf(rot_y);
      float pos_y = move_vec_y;
      float pos_z = move_vec_z;
      move_vec_y = pos_y * rot_cos - pos_z * rot_sin;
      move_vec_z = pos_z * rot_cos + pos_y * rot_sin;
    }

    {
      float rot_cos = cosf(rot_x);
      float rot_sin = sinf(rot_x);
      float pos_x = move_vec_x;
      float pos_z = move_vec_z;
      move_vec_x = pos_x * rot_cos - pos_z * rot_sin;
      move_vec_z = pos_z * rot_cos + pos_x * rot_sin;
    }

    pos_x += move_vec_x * 0.025;
    pos_y += move_vec_y * 0.025;
    pos_z += move_vec_z * 0.025;

    reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, vp_program);
    reiSetProgramLocalValue(ctx, REI_PROGRAM_BINDING_VERTEX, 0, pos_x, pos_y, pos_z, 0);
    reiSetProgramLocalValue(ctx, REI_PROGRAM_BINDING_VERTEX, 1, cosf(-rot_x), sinf(-rot_x), cosf(-rot_y), sinf(-rot_y));
    reiBindProgram(ctx, REI_PROGRAM_BINDING_VERTEX, 0);

    reiSubmitCommandLists(ctx, 1, &list);

    glfwSwapBuffers(window);

    mouse_x_prev = mouse_x;
    mouse_y_prev = mouse_y;
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
