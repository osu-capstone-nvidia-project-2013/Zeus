#include "objectclass.h"
#include "geometryclass.h"


ObjectClass::ObjectClass()
{
}

void ObjectClass::Render(ID3D11Device *dev, ID3D11DeviceContext *devcon, ID3D11RenderTargetView *backbuffer, IDXGISwapChain *swapchain)
{

    // clear the back buffer to a deep blue
    devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

        // select which vertex buffer to display
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;
        devcon->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);

		// set the index buffer
		devcon->IASetIndexBuffer(iBuffer, DXGI_FORMAT_R32_UINT, 0);
       
		// select which primtive type we are using
        devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // draw the vertex buffer to the back buffer
        devcon->DrawIndexed(numIndices, 0, 0);

    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);

}
